#include "Controller.h"
#include "App.h"

void Controller::clearRevisit() {
    while (!revisit.empty()) {
        View* view = revisit.top();
        revisit.pop();
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
        clearRevisit();
    }
    history.push(view);
    app->setNewState(view); 
}

void Controller::goBack() {
    if (history.size() <= 1) {  
        ERROR("controller", "view history empty");
        return;
    }
    View* current = history.top();
    history.pop();
    revisit.push(current);  
    app->setNewState(history.top());  
}

void Controller::goForward() {
    if (revisit.empty()) {
        ERROR("controller", "revisit is empty");
        return;
    }
    View* next = revisit.top();
    revisit.pop();
    history.push(next);  
    app->setNewState(next);  
}

void Controller::setupLoginView()
{
    LoginView *view = new LoginView(app);
    view->loginHandler = [this](const char *username, const char *password)
    {
        User *new_user = app->getDB()->signIn(username, password);
        if (new_user == nullptr)
        {
            ERROR("controller", "login failed");
            return;
        }

        app->changeUser(new_user);
        MainClientView *mainView = new MainClientView(app);
        pushView(mainView);
    };
    pushView(view);
}
