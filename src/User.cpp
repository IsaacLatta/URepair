#include "User.h"

User::User() : username(""), password(""), isLoggedIn(false) {}

User::User(const User& user) {
   this->username = user.username;
   this->password = user.password;
   this->jobs = user.jobs;
   this->isLoggedIn = user.isLoggedIn;
}

bool User::loadData() { 
   INFO("User", "fetching data ..."); 
   return true;
}

User* User::validate() {
   /*if(db->login())*/
   INFO("User", "validated");
   this->isLoggedIn = true;
   return new User(*this);
}

std::vector<Job>* User::getJobs() {
   return &jobs;
}

void User::setPassword(const char* password) {
    this->password = password;
 }

void User::setUsername(const char* username) {
    this->username = username;
 }

std::string User::getUsername() {
    return this->username;
 }

std::string User::getPassword() {
    return this->password;
 }