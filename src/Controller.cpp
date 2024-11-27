#include "Controller.h"

Controller::Controller() {}

/* Temp callback before moving into the Database class*/
static int query_callback(void* result_buffer, int argc, char** argv, char** col_name) {
	std::string* result = static_cast<std::string*>(result_buffer); 
	for (int i = 0; i < argc; i++) {
		*result += col_name[i];
        *result += " = ";
        *result += argv[i] ? argv[i] : "NULL";
        *result += "\n";
	}
	return 0;
}

void Controller::renderCurrent() {
    if(history.empty()) {
        ERROR("controller", "no view to render");
        return;
    }
    history.top()->render();
}

void Controller::pushView(std::shared_ptr<View> view) {
    if (!view) {
        ERROR("controller", "attempt to push NULL view");
        return;
    }
    if (!history.empty()) {
        /* User has now visited a new page, the revisit must be cleared to allow for new "goBack" operations */
        revisit = std::stack<std::shared_ptr<View>>(); 
    }
    history.push(view);
}

void Controller::goBack() {
    if (history.size() <= 1) {  
        ERROR("controller", "view history empty");
        return;
    }

    auto current_view = history.top();
    history.pop();
    revisit.push(current_view);   
}

void Controller::goForward() {
    if (revisit.empty()) {
        ERROR("controller", "revisit is empty");
        return;
    }
    auto next = revisit.top();
    revisit.pop();
    history.push(next);    
}

void Controller::setupLoginView() {
    auto view = std::make_shared<LoginView>();
    view->loginHandler = [this](const char *username, const char *password)
    {
        user = db->signIn(username, password);
        if (user == nullptr) {
            ERROR("controller", "login failed");
            return;
        }
        INFO("controller", "user logged in");
        setupMainView(user);
    };
    pushView(view);
    INFO("controller", "login view created");
}

bool Controller::start() {
    INFO("controller", "starting");
    db = Database::databaseFactory();
    if(!db->connect()) {
        return false;
    }
    setupLoginView();
    INFO("controller", "start up successfull");
    return true;
}

void Controller::setupMainContractorView(std::shared_ptr<User> user) {
    auto main = std::make_shared<MainContractorView>(user);
    main->logoutHandler = [this]() {
        setupLoginView();
    };
    main->jobAcceptHandler = [this, user](Job *job, bool approve) {
        std::cout << "CALLED\n";
        db->approveJob(user.get(), job, approve);
        db->loadData(user.get());
    };
    main->profileHandler = [this, user]() {
        setupProfileView(user);
    };
    pushView(main);
    LOG("INFO", "controller", "main contractor view pushed to history");
}

void Controller::setupMainClientView(std::shared_ptr<User> user) {
    auto main = std::make_shared<MainClientView>(user);

    main->searchHandler = [this, main](const char *service_type, const char *location, int min_rating, int min_price, int max_price) {
        auto search_results = db->findTalents(service_type, location, min_rating, min_price, max_price);
        std::sort(search_results.begin(), search_results.end(), [](const Talent& a, const Talent& b) {
        return a.rating > b.rating; 
        });

        main->search_results = search_results;
    };
    main->profileHandler = [this, main, user]() {
        setupProfileView(user);
    };
    main->logoutHandler = [this]() {
        INFO("profile state", "switching to login state");
        setupLoginView();
    };
    main->bookingHandler = [this, main, user](Talent *talent) {
        if(!db->bookJob(user.get(), talent) || !db->loadData(user.get())) {
            LOG("INFO", "controller", "job booking failed"); 
        }
    };
    main->uploadHandler = [this]() {
        INFO("booking", "uploaded"); 
    };

    INFO("controller", "handlers assigned to main client view, pushed view");
    pushView(main);
}

void Controller::setupProfileView(std::shared_ptr<User> user) {
    if(user->role != ROLE::CLIENT && user->role != ROLE::CONTRACTOR) {
        LOG("ERROR", "controller", "user role: %d is invalid", static_cast<int>(user->role));
        return;
    }

    //auto profile = std::make_shared<ClientProfileView>(user);
    auto profile = std::make_shared<ProfileView>(user);
    profile->changeUsername = [this, user](const char* username, const char* password) 
    {
        return db->changeUsername(user.get(), username, password);
    };
    profile->changePassword = [this, user](const char* new_password, const char* old_password) {
        return db->changePassword(user.get(), new_password, old_password);
    };
    profile->logoutHandler = [this]() {
        setupLoginView();
    };
    profile->goBack = [this]() {
        goBack();
    };
    profile->updateInfoHandler = [this, user](char* field, char* new_val) {
        db->changeInfo(user.get(), std::string(field).c_str(), std::string(new_val).c_str());
        db->loadData(user.get());
    };
    profile->updateTalentInfoHandler = [this, user](char* field, char* new_val) {
        std::cout << "updating talent\n";
        db->changeInfo(user.get(), std::string(field).c_str(), std::string(new_val).c_str(), user->getTalent());
        db->loadData(user.get());
    };
    pushView(profile);
}

void Controller::setupMainView(std::shared_ptr<User> user) {
    if (user->role == ROLE::CLIENT) {
        setupMainClientView(user);   
    }
    else if (user->role == ROLE::CONTRACTOR) {
        setupMainContractorView(user);
    }
    else if (user->role == ROLE::ADMIN) {
        auto main = std::make_shared<MainAdminView>(user);
        main->queryHandler = [this](const char* query) {
            LOG("INFO", "controller", "running query: %s", query);

            std::string error_msg;
            std::string result_buf;
            std::string query_str(query);
            if(!Database::runQuery(query_str, query_callback,(void*)(&result_buf), error_msg)) {
                LOG("ERROR", "controller", "query failed: %s", error_msg.c_str());
                return error_msg;
            }
            
            return result_buf;
        };
        main->logoutHandler = [this]() 
        {
            INFO("profile state", "switching to login state");
            setupLoginView();
        };
        pushView(main);
    }
    else {
        ERROR("controller", "user dynamic_cast failed");
        return;
    }

    db->loadData(user.get());
    INFO("controller", "user data loaded");
}

