#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include <memory>
#include <cstring>
#include <sqlite3.h>

#define SQLITE_FILE "../URepair.db"

/**
 * @class Database
 * @brief Abstract base class representing a generic interface for a database system.
 *
 * This class defines the core functionalities required for user management,
 * job booking, data loading, and other operations. It is designed to be 
 * inherited by specific database implementations like SQLite or dummy databases.
 */
class Database 
{
public:
    /**
     * @brief Factory method to create an instance of a database.
     * 
     * @return A unique pointer to a concrete implementation of the Database class.
     */
    static std::unique_ptr<Database> databaseFactory();

    /**
     * @brief Executes a raw SQL query using the underlying database.
     * 
     * @param query The SQL query string to execute.
     * @param callback A callback function to process query results.
     * @param callback_param Pointer to the parameter passed to the callback.
     * @param error_msg Reference to a string for capturing error messages.
     * @return True if the query executes successfully, false otherwise.
     */
    static bool runQuery(const std::string& query, int(*callback)(void*, int, char**, char**), 
                         void* callback_param, std::string& error_msg);

    /**
     * @brief Connects to the database.
     * 
     * @return True if the connection is successful, false otherwise.
     */
    virtual bool connect() = 0;

    /**
     * @brief Signs in a user with the given credentials.
     * 
     * @param username The username of the user.
     * @param password The password of the user.
     * @return A shared pointer to the authenticated User object, or nullptr on failure.
     */
    virtual std::shared_ptr<User> signIn(const char* username, const char* password) = 0;

    /**
     * @brief Books a job between a user and a talent.
     * 
     * @param user Pointer to the user requesting the job.
     * @param talent Pointer to the talent providing the service.
     * @return True if the job is successfully booked, false otherwise.
     */
    virtual bool bookJob(User* user, Talent* talent) = 0;

    /**
     * @brief Loads additional data for a given user from the database.
     * 
     * @param user Pointer to the user whose data is being loaded.
     * @return True if data is successfully loaded, false otherwise.
     */
    virtual bool loadData(User* user) = 0;

    /**
     * @brief Changes the password of a user.
     * 
     * @param user Pointer to the user.
     * @param old_pass The current password.
     * @param new_pass The new password.
     * @return True if the password is successfully changed, false otherwise.
     */
    virtual bool changePassword(User* user, const char* old_pass, const char* new_pass) = 0;

    /**
     * @brief Changes the username of a user.
     * 
     * @param user Pointer to the user.
     * @param password The current password for verification.
     * @param new_username The desired new username.
     * @return True if the username is successfully changed, false otherwise.
     */
    virtual bool changeUsername(User* user, const char* password, const char* new_username) = 0;

    /**
     * @brief Updates user or talent information in the database.
     * 
     * @param user Pointer to the user.
     * @param what The field to be updated.
     * @param newInfo The new value for the field.
     * @param talent Pointer to the talent object (optional).
     * @return True if the information is successfully updated, false otherwise.
     */
    virtual bool changeInfo(User* user, const char* what, const char* newInfo, Talent* talent = nullptr) = 0;

    /**
     * @brief Searches for talents based on specific criteria.
     * 
     * @param service_type The type of service the talent offers (optional).
     * @param location The location of the talent (optional).
     * @param min_rating The minimum acceptable rating (default: 0).
     * @param min_price The minimum acceptable price (default: 0).
     * @param max_price The maximum acceptable price (default: 10000).
     * @return A vector of talents matching the criteria.
     */
    virtual std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) = 0;

    /**
     * @brief Approves or denies a job request.
     * 
     * @param user Pointer to the user.
     * @param job Pointer to the job being reviewed.
     * @param approve True to approve the job, false to deny.
     * @return True if the job status is successfully updated, false otherwise.
     */
    virtual bool approveJob(User* user, Job* job, bool approve) = 0;

    /**
     * @brief Virtual destructor for the Database class.
     */
    virtual ~Database() = default;
};

/**
 * @class Dummy
 * @brief A mock implementation of the Database class for testing purposes.
 */
class Dummy : public Database 
{
public:
    // Overrides for all virtual methods with dummy implementations
    bool connect() override;
    std::shared_ptr<User> signIn(const char* username, const char* password) override;
    bool bookJob(User* user, Talent* talent) override;
    bool loadData(User* user) override;
    bool changePassword(User* user, const char* old_pass, const char* new_pass) override;
    bool changeUsername(User* user, const char* password, const char* new_username) override;
    bool changeInfo(User* user, const char* what, const char* newInfo, Talent* talent = nullptr) override;
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) override;
    bool approveJob(User* user, Job* job, bool approve) override;
};

/**
 * @class SQLite
 * @brief A concrete implementation of the Database class using SQLite.
 */
class SQLite : public Database
{
public:
    // Overrides for all virtual methods with SQLite-specific implementations
    bool connect() override;
    std::shared_ptr<User> signIn(const char* username, const char* password) override;
    bool loadData(User* user) override;
    bool bookJob(User* user, Talent* talent) override;
    bool changePassword(User* user, const char* old_pass, const char* new_pass) override;
    bool changeUsername(User* user, const char* password, const char* new_username) override;
    bool changeInfo(User* user, const char* what, const char* newInfo, Talent* talent = nullptr) override;
    std::vector<Talent> findTalents(const char* service_type = "", const char* location = "", int min_rating = 0, int min_price = 0, int max_price = 10000) override;
    bool approveJob(User* user, Job* job, bool approve) override;
};

#endif
