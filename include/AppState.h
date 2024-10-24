#ifndef APPSTATE_H
#define APPSTATE_H

class App;

class AppState 
{
    public:
    AppState(App* app): app(app) {};
    virtual void handle() = 0;
    protected:
    App* app;
};

class MainState: public AppState {
    public:
    MainState(App* app) : AppState(app) {};
    void handle() override;
    private:
    void showMenu(bool*);
    void showMainWindow();
};

class LoginState: public AppState
{   
    public:
    LoginState(App* app): AppState(app) {};
    void handle() override;
};

#endif