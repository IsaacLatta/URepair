#include "Controller.h"
#include "App.h"

void Controller::clearViews(std::stack<View*>& views) {
    while (!views.empty()) {
        View* view = views.top();
        views.pop();
        delete view; 
    }
}

void Controller::renderCurrent() {
    if(history.empty()) {
        ERROR("controller", "no view to render");
        return;
    }
    history.top()->handle();
}

void Controller::pushView(View* view) {
    if (!view) {
        ERROR("controller", "attempt to push NULL view");
        return;
    }
    if (!history.empty()) {
        clearViews(revisit);
        revisit = std::stack<View*>();
    }
    history.push(view);
}

void Controller::goBack() {
    if (history.size() <= 1) {  
        ERROR("controller", "view history empty");
        return;
    }
    View* current = history.top();
    history.pop();
    revisit.push(current);   
}

void Controller::goForward() {
    if (revisit.empty()) {
        ERROR("controller", "revisit is empty");
        return;
    }
    View* next = revisit.top();
    revisit.pop();
    history.push(next);    
}

void Controller::setupLoginView()
{
    clearViews(history);
    clearViews(revisit);
    LoginView *view = new LoginView(app);
    view->loginHandler = [this](const char *username, const char *password)
    {
        User *new_user = app->getDB()->signIn(username, password);
        if (new_user == nullptr)
        {
            ERROR("controller", "login failed");
            return;
        }
        setupMainView(new_user);
    };
    pushView(view);
}

void Controller::setupProfileView(User* user) {
    // To be implemented
    ProfileView* profile = new ProfileView(app);
}

void Controller::setupMainView(User *user) {
    if (dynamic_cast<Client *>(user))
    {
        MainClientView *main = new MainClientView(app);
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
        pushView(main);
    }
    else
    {
        ERROR("controller", "cannot render MainClientView");
        return;
    }

    app->changeUser(user);
}

