#include "User.h"

User::User() : username(""), password(""), isLoggedIn(false) {}

Info* User::getInfo() {
   return &info;
}

void User::setJobs(std::vector<Job>& jobs) {
   this->jobs = std::move(jobs);
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

void User::setInfo(Info&& info) {
    this->info = std::move(info);
}

void Contractor::setTalent(Talent talent) {
    this->talent = talent;
}

Talent* Contractor::getTalent() {
    return &talent;
}

