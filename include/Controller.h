#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stack>
#include "logger.h"

class App;
class View;

class Controller
{
    public:
    Controller(App* app): app(app) {}
    void renderCurrent();
    void setupLoginView();
    private:
    void clearRevisit();
    void pushView(View* view);
    void goBack();
    void goForward();
    App* app;
    std::stack<View*> history;
    std::stack<View*> revisit;
};

#endif