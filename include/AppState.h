#ifndef APPSTATE_H
#define APPSTATE_H
#include <vector>

#include "User.h"

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
    char search_service_type[128] = "";  
    char search_location[128] = "";      
    int min_rating = 0;                  
    int min_price = 0;                   
    int max_price = 500;                 

    std::vector<Talent> search_results;
    void showMenuBar();
    void showJobs();
    void showTalentSearchFilters();
    void showTalentSearchResults();
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