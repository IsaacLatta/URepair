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

class User 
{
    public:
    bool isLoggedIn;
    User();
    std::vector<Job>* getJobs();
    void setUsername(const char*);
    void setPassword(const char*);
    bool loadData();
    std::string getUsername();
    std::string getPassword();
    private:
    std::vector<Job> jobs;
    std::string username;
    std::string password;
};


#endif