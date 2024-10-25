#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "logger.h"

struct Job {
    int id;
    char description[256];
    char name[128];
    char status[128];
    char date[256];
    double cost;
} ;

struct Talent {
    std::string name;
    std::string service_type;  
    std::string location;      
    int rating;                // Rating out of 5
    float rate;
};

class User 
{
    public:
    bool isLoggedIn;

    User();
    User(const User& user);

    std::vector<Job>* getJobs();
    void setUsername(const char*);
    void setPassword(const char*);
    User* validate();
    void setJobs(std::vector<Job>& jobs);
    std::string getUsername();
    std::string getPassword();

    protected:
    std::vector<Job> jobs;
    std::string username;
    std::string password;
};


#endif