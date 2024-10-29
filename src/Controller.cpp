#include "Controller.h"

Controller::Controller() {}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void run_query(const char* query) {
    sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	const char* data = "Callback function called";
	
	
	rc = sqlite3_open("test.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	//sql = "CREATE TABLE users(userID int, fname varchar, lname varchar,"
	//"dob date)";
	//sql = "Insert into users values(4, 'Bradley', 'Schmidt', '2004-05-22')";
	sql = "select * from users";

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);
}

void Controller::renderCurrent() {
    if(history.empty()) {
        ERROR("controller", "no view to render");
        return;
    }
    auto current = history.top();
    current->handle();
}

void Controller::pushView(std::shared_ptr<View> view) {
    if (!view) {
        ERROR("controller", "attempt to push NULL view");
        return;
    }
    if (!history.empty()) {
        revisit = std::stack<std::shared_ptr<View>>();
    }
    history.push(view);
}

void Controller::goBack() {
    if (history.size() <= 1) {  
        ERROR("controller", "view history empty");
        return;
    }
    auto current = history.top();
    history.pop();
    revisit.push(current);   
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
    INFO("controller", "start up successful");
    return true;
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
    if (auto client = std::dynamic_pointer_cast<Client>(user))
    {
        auto main = std::make_shared<MainClientView>(client);
        main->searchHandler = [this, main](const char *service_type, const char *location, int min_rating, int min_price, int max_price)
        {
            INFO("Search", "Search for talent triggered");
            main->search_results = db->findTalents(service_type, location, min_rating, min_price, max_price);
        };
        main->profileHandler = [this, main, user]()
        {
            setupProfileView(user);
        };
        main->logoutHandler = [this]()
        {
            INFO("profile state", "switching to login state");
            setupLoginView();
        };
        main->bookingHandler = [this, main, user](Talent* talent)
        {
            INFO("booking", "talent booked");  // to be implemented
        };
        main->uploadHandler = [this]()
        {
            INFO("booking", "uploaded");  // to be implemented
        };
        INFO("controller", "handlers assigned to main client view, pushing new view");
        pushView(main);
        INFO("controller", "main view pushed");
    }
    else if (auto contr = std::dynamic_pointer_cast<Contractor>(user))
    {
        auto main = std::make_shared<MainContractorView>(contr);
        main->logoutHandler = [this]() 
        {
            setupLoginView();
        };
        main->jobAcceptHandler = [this, contr](Job* job) 
        {
            if(db->bookJob(contr.get(), job)) {
                INFO("controller", "job booked");
            }
        };
        main->profileHandler = [this]()
        {
            INFO("controller", "would switch to client view");
        };
        pushView(main);
        LOG("INFO", "controller", "main contractor view pushed to history");
    }
    else if (auto admin = std::dynamic_pointer_cast<Admin>(user)) {
        auto main = std::make_shared<MainAdminView>(admin);
        main->queryHandler = [this](const char* query) 
        {
            LOG("INFO", "controller", "running query: %s", query);
            return "";
        };
        pushView(main);
    }
    else
    {
        ERROR("controller", "user dynamic_cast failed");
        return;
    }

    db->loadData(user.get());
    INFO("controller", "user data loaded");
}

