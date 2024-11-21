#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include <memory>
#include <cstring>
#include <sqlite3.h>

#define SQLITE_FILE "../test.db"

class Database 
{
    public:
    static std::unique_ptr<Database> databaseFactory();
    static bool runQuery(const std::string& query, int(*callback)(void*,int,char**,char**), 
                        void* callback_param, std::string& error_msg);

    virtual bool connect() = 0;
    virtual std::shared_ptr<User> signIn(const char* username, const char* password) = 0; // factory function that creates the corresponding user, (e.g. client, talent) may vary for sqlite vs sql server vs dummy.
    virtual bool bookJob(User*, Job*) = 0;
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
    std::shared_ptr<User> signIn(const char* username, const char* password) override;
    bool bookJob(User*, Job*) override;
    bool loadData(User*) override;
    void updateJobs(User*) override;
    bool changePassword(User* user, const char* old_pass, const char* new_pass) override;
    bool changeUsername(User* user, const char* password, const char* new_username) override;
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) override;
};

/*
class SQLite : public Database
{
    public:
    bool connect() override;
     
    std::shared_ptr<User> signIn(const char* username, const char* password) override;
    bool loadData(User*) override;
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) override; 
    
    bool changePassword(User* user, const char* old_pass, const char* new_pass) override;
    bool changeUsername(User* user, const char* password, const char* new_username) override;
};
*/

#endif