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
    virtual ~View() = default;
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
    std::vector<Talent> search_results;
    std::function<void(const char*, const char*, int, int, int)> searchHandler;
    std::function<void()> logoutHandler;
    std::function<void()> profileHandler;
    std::function<void(Talent*)> bookingHandler;
    std::function<void()> uploadHandler;

    MainClientView(App* app) : View(app) {};
    MainClientView(App* app, Client* client) : View(app), client(client) {};
    void handle() override;
    private:
    Client* client;

    char search_service_type[128] = "";  
    char search_location[128] = "";      
    int min_rating = 0;                  
    int min_price = 0;                   
    int max_price = 500;                 

    void showMenuBar();
    void showJobs();
    void showTalentSearchFilters();
    void showTalentSearchResults();
    void showBookingMenu(Talent* talent, bool*);
};


class ProfileView: public View {
    public:
    std::function<bool(const char*, const char*)> changeUsername;
    std::function<bool(const char*, const char*)> changePassword;

    ProfileView(App* app): View(app) {};
    void handle() override;
    private:
    void showOptions();
    void showSecurityMenu(bool*);
};



#endif