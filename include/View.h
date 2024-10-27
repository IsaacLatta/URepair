#ifndef APPSTATE_H
#define APPSTATE_H
#include <vector>

#include "User.h"
#include <functional>

class App;

class View 
{
    public:
    View(App* app): app(app) {};
    virtual void handle() = 0;
    protected:
    App* app;
};

class LoginView: public View
{   
    public:
    std::function<void(const char*, const char*)> loginHandler;
    LoginView(App* app): View(app) {};
    void handle() override;
};

class MainClientView: public View {
    public:
    MainClientView(App* app) : View(app) {};
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
    void showBookingMenu(Talent* talent, bool*);
};


class ProfileView: public View {
    public:
    ProfileView(App* app): View(app) {};
    void handle() override;
    private:
    void showOptions();
    void showSecurityMenu(bool*);
};



#endif