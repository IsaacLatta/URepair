#include "User.h"

User::User() : username(""), password(""), isLoggedIn(false) {}

bool User::loadData() { 
   INFO("User", "fetching data ..."); 
   return true;
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