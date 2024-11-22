#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "logger.h"

/**
 * @enum ROLE
 * @brief Represents the role of a user in the application.
 */
enum class ROLE {
    UNKNOWN,    ///< Undefined role.
    CLIENT,     ///< A client user.
    CONTRACTOR, ///< A contractor user.
    ADMIN       ///< An admin user.
};

/**
 * @struct Job
 * @brief Represents a job entity.
 */
struct Job {
    int id;                 ///< Unique identifier for the job.
    char description[256];  ///< Description of the job.
    char name[128];         ///< Name associated with the job.
    char status[128];       ///< Current status of the job.
    char date[256];         ///< Date associated with the job.
    double cost;            ///< Cost of the job.
};

/**
 * @struct Talent
 * @brief Represents a talent offering services.
 */
struct Talent {
    int id;                  ///< Unique identifier for the talent.
    std::string name;        ///< Name of the talent.
    std::string service_type;///< Type of service provided by the talent.
    std::string location;    ///< Location of the talent.
    int rating;              ///< Rating of the talent.
    float rate;              ///< Service rate of the talent.

    Talent() {}
    Talent(const char* name, const char* service, const char* location, int rating, float rate) :
    name(name), service_type(service), location(location), rating(rating), rate(rate){}
};

/**
 * @struct Info
 * @brief Represents additional information about a user.
 */
struct Info {
    int id;                 ///< Unique identifier for the info.
    std::string phone;      ///< Phone number of the user.
    std::string email;      ///< Email address of the user.
    std::string name;       ///< Full name of the user.
    std::string location;   ///< Location of the user.
    std::string bio;        ///< User's biography.

    Info() : id(-1) {}
    Info(const char* email, const char* phone, const char* name, const char* loc, const char* bio) :
    email(email), phone(phone), name(name), location(loc), bio(bio) {}
};

/**
 * @struct Message
 * @brief Represents a message exchanged with a talent.
 */
struct Message {
    Talent talent;          ///< Talent associated with the message.
    std::string message;    ///< Content of the message.
};

/**
 * @class User
 * @brief Represents a user entity in the application.
 *
 * The `User` class is the primary model for storing user-related data
 * such as their credentials, roles, jobs, messages, and talents.
 */
class User 
{
public:
    ROLE role;           ///< Role of the user in the application.
    bool isLoggedIn;     ///< Login status of the user.

    /**
     * @brief Default constructor for creating an uninitialized user.
     */
    User();

    /**
     * @brief Constructor for creating a user with a username and password.
     * @param username The user's username.
     * @param password The user's password.
     */
    User(const char* username, const char* password);

    /**
     * @brief Virtual destructor for the User class.
     */
    virtual ~User() = default;

    // Setters
    void setTalent(Talent&& talent);
    void setJobs(std::vector<Job>& jobs);
    void setMessages(std::vector<Message>&& messages);
    void setUsername(const char* username);
    void setPassword(const char* password);
    void setInfo(Info&& info);

    // Getters
    std::vector<Job>* getJobs();
    Talent* getTalent();
    std::vector<Message>* getMessages();
    Info* getInfo();
    std::string getUsername();
    std::string getPassword();

protected:
    Talent talent;               ///< Talent associated with the user.
    Info info;                   ///< Additional information about the user.
    std::vector<Message> messages; ///< Messages related to the user.
    std::vector<Job> jobs;       ///< Jobs assigned to or requested by the user.
    std::string username;        ///< Username of the user.
    std::string password;        ///< Password of the user.
};

#endif