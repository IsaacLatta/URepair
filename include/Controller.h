#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <stack>

#include "View.h"
#include "User.h"
#include "logger.h"
#include "Database.h"
#include "ThreadPool.h"


class Controller
{
    public:
    Controller();
    bool start();
    void renderCurrent();
    private:
    void setupLoginView();
    void setupMainView(std::shared_ptr<User> user);
    void setupProfileView(std::shared_ptr<User> user);
    void pushView(std::shared_ptr<View> view);
    void goBack();
    void goForward();
    ThreadPool pool;
    std::shared_ptr<User> user;
    std::unique_ptr<Database> db;
    std::stack<std::shared_ptr<View>> history;
    std::stack<std::shared_ptr<View>> revisit;
};

#endif