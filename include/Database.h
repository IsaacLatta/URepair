#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include <cstring>

class Database 
{
    public:
    static Database* databaseFactory();
    virtual User* validate(User* user) = 0; // factory function that creates the corresponding user, (e.g. client, talent) may vary for sqlite vs sql server vs dummy.
    virtual bool loadData(User* user) = 0;
    virtual void updateJobs(User* user) = 0;
    virtual std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) = 0;
    virtual ~Database() = default;
};

class Dummy : public Database 
{
    public:
    User* validate(User*);
    bool loadData(User*);
    void updateJobs(User*);
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000);
};

#endif