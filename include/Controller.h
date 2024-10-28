#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <stack>
#include "logger.h"


class User;
class App;
class View;

class Controller
{
    public:
    Controller(App* app): app(app) {}
    void renderCurrent();
    void setupLoginView();
    private:
    void setupMainView(User*);
    void setupProfileView(User*);
    void pushView(std::shared_ptr<View> view);
    void goBack();
    void goForward();
    App* app;
    std::stack<std::shared_ptr<View>> history;
    std::stack<std::shared_ptr<View>> revisit;
};

#endif