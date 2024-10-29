#ifndef VIEW_H
#define VIEW_H
#include <vector>

#include "User.h"
#include <memory>
#include <functional>

#define BUFFER_SIZE 256

class User;

class View 
{
    public:
    View(): user(nullptr) {}
    View(std::shared_ptr<User> user): user(user) {};
    virtual void handle() = 0;
    virtual ~View() = default;
    protected:
    std::shared_ptr<User> user;
};

class LoginView: public View
{   
    public:
    std::function<void(const char*, const char*)> loginHandler = nullptr;
    LoginView(std::shared_ptr<User> user): View(user) {};
    LoginView() {}
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

    explicit MainClientView(std::shared_ptr<Client> client) : View(client) {}
    void handle() override;
    private:
    
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
    std::function<void()> logoutHandler;
    std::function<void()> goBack;

    ProfileView(std::shared_ptr<User> user): View(user) {}
    void handle() override;
    private:
    void showOptions();
    void showSecurityMenu(bool*);
    void showProfileInfo();
    void showEditOptions(bool*);
};


class MainContractorView: public View {
public:
    explicit MainContractorView(std::shared_ptr<Contractor> contractor) : View(contractor) {};
    void handle() override;
    std::function<void(Job*)> jobAcceptHandler = [](Job*) { INFO("view", "would call accept job"); }; // needs to be created - allows the contractors to view incoming requests
    std::function<void()> logoutHandler = []() { INFO("view", "would log user out"); };
    std::function<void()> profileHandler = []() { INFO("view", "user can view profile"); }; // 
    //std::function<void()> activejobHandler = []() { INFO("view", "see open jobs "); }; // needs to be created - allows contractors to view details about current active accepted jobs

private:

   void showContractorMenuBar();
   void showJobRequests();
  
    
};

#endif