#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "logger.h"

enum class ROLE {
    UNKNOWN, CLIENT, CONTRACTOR, ADMIN
};

struct Job {
    int id;
    char description[256];
    char name[128];
    char status[128];
    char date[256];
    double cost;
};

struct Talent {
    std::string name;
    std::string service_type;  
    std::string location;      
    int rating;                
    float rate;
    Talent() {}
    Talent(const char* name, const char* service, const char* location, int rating, float rate): 
    name(name), service_type(service), location(location), rating(rating), rate(rate){}
};

struct Info {
    int id;
    std::string phone;
    std::string email;
    std::string name;
    std::string location;
    std::string bio;
    Info() : id(-1) {} 
    Info(const char* email, const char* phone, const char* name, const char* loc, const char* bio):
    email(email), phone(phone), name(name), location(loc), bio(bio) {}
};

struct Message {
    Talent talent;
    std::string message;
};

class User 
{
    public:
    ROLE role;
    bool isLoggedIn;
    User();
    User(const char* username, const char* password): isLoggedIn(true), username(username), password(password) {}
    virtual ~User() = default;
    void setTalent(Talent&& talent);

    std::vector<Job>* getJobs();
    Talent* getTalent();
    std::vector<Message>* getMessages();
    void setMessages(std::vector<Message>&&);
    void setUsername(const char*);
    void setPassword(const char*);
    void setJobs(std::vector<Job>& jobs);
    void setInfo(Info&&);
    std::string getUsername();
    std::string getPassword();
    Info* getInfo();
    
    protected:
    Talent talent;
    Info info;
    std::vector<Message> messages;
    std::vector<Job> jobs;
    std::string username;
    std::string password;
};

#endif