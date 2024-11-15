#include "Controller.h"

Controller::Controller() {}

static int callback(void* result_buffer, int argc, char** argv, char** col_name) {
	std::string* result = static_cast<std::string*>(result_buffer); 
	for (int i = 0; i < argc; i++) {
		*result += col_name[i];
        *result += " = ";
        *result += argv[i] ? argv[i] : "NULL";
        *result += "\n";
	}
    std::cout << "CALLED\n";
	return 0;
}

const char* run_query(const char* query) {
    sqlite3* db;
	char* error_msg = 0;
	int ret_code;
	std::string result = "";
	
    LOG("INFO", "run_query", "Query received: %s", query);

	ret_code = sqlite3_open("../test.db", &db);
	if (ret_code) {
        LOG("ERROR", "controller", "cannot open database: %s", sqlite3_errmsg(db));
        char* ret_result = new char[BUFFER_SIZE];
        snprintf(ret_result, BUFFER_SIZE, "Database fail to open, code=%d (%s)", ret_code, sqlite3_errmsg(db));
        return ret_result;
	}
	LOG("INFO", "controller", "database open with code: %d", ret_code);

	ret_code = sqlite3_exec(db, query, callback, (void*)&result, &error_msg);
	if (ret_code != SQLITE_OK) {
        LOG("ERROR", "controller", "Query failed: %s", error_msg);
        char* ret_result = new char[strlen(error_msg) + 1];
        std::strcpy(ret_result, error_msg);
        
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return ret_result;
	}
	LOG("INFO", "controller", "query executed with code: %d", ret_code);

	INFO("controller", "query successful");
	sqlite3_close(db);
    
    char* ret_result = new char[result.length() + 1];
    std::strcpy(ret_result, result.c_str());
    LOG("INFO", "controller", "Query result content: %s, Size of result: %d", ret_result, result.length());
    return ret_result;
}

void Controller::renderCurrent() {
    if(history.empty()) {
        ERROR("controller", "no view to render");
        return;
    }
    auto current = history.top();
    current->render();
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

    // Remove the current view from   
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

void Controller::setupLoginView()
{
    auto view = std::make_shared<LoginView>();
    view->loginHandler = [this](const char *username, const char *password)
    {
        INFO("controller", "logging user in");
        user = db->signIn(username, password);
        if (user == nullptr)
        {
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

void Controller::setupMainContractorView(std::shared_ptr<Contractor> contr)
{
    auto main = std::make_shared<MainContractorView>(contr);
    main->logoutHandler = [this]() {
        setupLoginView();
    };
    main->jobAcceptHandler = [this, contr](Job *job) {
        if (db->bookJob(contr.get(), job))
        {
            INFO("controller", "job booked");
        }
    };
    main->profileHandler = [this]() {
        INFO("controller", "would switch to client view");
    };
    pushView(main);
    LOG("INFO", "controller", "main contractor view pushed to history");
}

void Controller::setupMainClientView(std::shared_ptr<Client> client) {
    auto main = std::make_shared<MainClientView>(client);
    main->searchHandler = [this, main](const char *service_type, const char *location, int min_rating, int min_price, int max_price) {
        INFO("Search", "Search for talent triggered");
        main->search_results = db->findTalents(service_type, location, min_rating, min_price, max_price);
    };
    main->profileHandler = [this, main, client]() {
        setupProfileView(client);
    };
    main->logoutHandler = [this]() {
        INFO("profile state", "switching to login state");
        setupLoginView();
    };
    main->bookingHandler = [this, main, client](Talent *talent) {
        INFO("booking", "talent booked"); // to be implemented
    };
    main->uploadHandler = [this]() {
        INFO("booking", "uploaded"); // to be implemented
    };

    INFO("controller", "handlers assigned to main client view, pushing new view");
    pushView(main);
    INFO("controller", "main view pushed");
}

void Controller::setupProfileView(std::shared_ptr<User> user) {
    if(auto client = std::dynamic_pointer_cast<Client>(user)) {
        auto profile = std::make_shared<ClientProfileView>(user);
        profile->changeUsername = [this, user](const char* username, const char* password) 
        {
            INFO("controller", "changing username");
            return db->changeUsername(user.get(), username, password);
        };
        profile->changePassword = [this, user](const char* new_password, const char* old_password) 
        {
            INFO("controller", "changing password");
            return db->changePassword(user.get(), new_password, old_password);
        };
        profile->logoutHandler = [this]()
        {
            INFO("controller", "switching to login state");
            setupLoginView();
        };
        profile->goBack = [this]()
        {
            INFO("controller", "going back");
            goBack();
        };
        pushView(profile);
    }
    else if (auto contr = std::dynamic_pointer_cast<Contractor>(user)) {
        INFO("controller", "attempt to render contractor profile");
    }
    else {
        ERROR("controller", "user is not a client");
    }
}

void Controller::setupMainView(std::shared_ptr<User> user) {
    if (auto client = std::dynamic_pointer_cast<Client>(user)) {
        setupMainClientView(client);   
    }
    else if (auto contr = std::dynamic_pointer_cast<Contractor>(user)) {
        setupMainContractorView(contr);
    }
    else if (auto admin = std::dynamic_pointer_cast<Admin>(user)) {
        auto main = std::make_shared<MainAdminView>(admin);
        main->queryHandler = [this](const char* query) {
            LOG("INFO", "controller", "running query: %s", query);
            return run_query(query);

            /* sudo code
            auto result = co_await threadpool->exec(db->runQuery, query);
            user->update(result);
            */
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

