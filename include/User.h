#ifndef USER_H
#define USER_H

#include <string>

class User 
{
    public:
    bool isLoggedIn;
    User();
    void setUsername(const char*);
    void setPassword(const char*);
    std::string getUsername();
    std::string getPassword();
    private:
    std::string username;
    std::string password;
};


#endif