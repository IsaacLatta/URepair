#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include <cstring>

class Database 
{
    public:
    static Database* databaseFactory();
    virtual bool connect() = 0;
    virtual User* validate(User* user) = 0; // factory function that creates the corresponding user, (e.g. client, talent) may vary for sqlite vs sql server vs dummy.
    virtual bool loadData(User* user) = 0;
    virtual void updateJobs(User* user) = 0;
    virtual bool changePassword(User* user, const char*, const char*) = 0;
    virtual bool changeUsername(User* user, const char*, const char*) = 0;
    virtual std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) = 0;
    virtual ~Database() = default;
};

class Dummy : public Database 
{
    public:
    bool connect() override;
    User* validate(User*) override;
    bool loadData(User*) override;
    void updateJobs(User*) override;
    bool changePassword(User* user, const char* old_pass, const char* new_pass) override;
    bool changeUsername(User* user, const char* password, const char* new_username) override;
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) override;
};




#endif