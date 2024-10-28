#include "Controller.h"
#include "App.h"

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
    auto view = std::make_shared<LoginView>(app);
    view->loginHandler = [this](const char *username, const char *password)
    {
        INFO("controller", "logging user in");
        User *new_user = app->getDB()->signIn(username, password);
        if (new_user == nullptr)
        {
            ERROR("controller", "login failed");
            return;
        }
        INFO("controller", "user logged in");
        setupMainView(new_user);
    };
    pushView(view);
}

void Controller::setupProfileView(User* user) {
    if(auto* client = dynamic_cast<Client*>(user)) {
        auto profile = std::make_shared<ProfileView>(app);
        profile->changeUsername = [this, user](const char* username, const char* password) 
        {
            INFO("controller", "changing username");
            return app->getDB()->changeUsername(user, username, password);
        };
        profile->changePassword = [this, user](const char* new_password, const char* old_password) 
        {
            INFO("controller", "changing password");
            return app->getDB()->changePassword(user, new_password, old_password);
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
    else {
        ERROR("controller", "user is not a client");
    }
}

void Controller::setupMainView(User *user) {
    if (auto* client = dynamic_cast<Client *>(user))
    {
        auto main = std::make_shared<MainClientView>(app, client);
        main->searchHandler = [this, main](const char *service_type, const char *location, int min_rating, int min_price, int max_price)
        {
            INFO("Search", "Search for talent triggered");
            main->search_results = app->getDB()->findTalents(service_type, location, min_rating, min_price, max_price);
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
        INFO("controller", "handlers assigned to main view, pushing new view");
        pushView(main);
        INFO("controller", "main view pushed");
    }
    else
    {
        ERROR("controller", "cannot render MainClientView");
        return;
    }

    app->getDB()->loadData(user);
    INFO("controller", "user data loaded");
}

