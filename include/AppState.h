#ifndef APPSTATE_H
#define APPSTATE_H
#include <vector>

class App;

class AppState 
{
    public:
    AppState(App* app): app(app) {};
    virtual void handle() = 0;
    protected:
    App* app;
};

class LoginState: public AppState
{   
    public:
    LoginState(App* app): AppState(app) {};
    void handle() override;
};

class MainState: public AppState {
    public:
    MainState(App* app) : AppState(app) {};
    void handle() override;
    private:
    void showMenuBar();
    void showJobs();
};

/*
class ProfileState: public AppState {
    public:
    ProfileState(App* app): AppState(app) {};
    void handle() override;
    private:

};
*/


#endif