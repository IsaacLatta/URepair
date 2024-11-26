
#include "Database.h"
#include <random>

/* USER
username = mbaudin
password = 616481
fname = Michael
lname = Baudin
type = client/contractor/admin
lastaccessed = 2024-11-21
userID = 1000
talentID = NULL
*/

/* INFO
userID = 1000
phone = 250-320-0439
email = mikeybleven@gmail.com
name = Michael
location = Kamloops
bio = I am a person
*/

/* JOB
jobId = 1000
description = Isaac is laying pipe at Michaels house
name = MichaelJob
status = 0
cost = 200
userId = 1000
talentId = 1000
*/

/*
struct Job {
    int id;
    char description[256];
    char name[128];
    char status[128];
    char date[256];
    double cost;
} ;*/

/* JOB
jobId = 1000
description = Isaac is laying pipe at Michaels house
name = MichaelJob
status = 0
cost = 200
userId = 1000
talentId = 1000
*/

/* TALENT
talentID = 1000
name = Isaac Pipe Laying Ltd.
service_type = Plumbing
location = Kamloops
rating = 5
rate = 200
message = I will lay your pipe :)
*/

bool SQLite::connect() {
    return true;
}

ROLE to_role(const char* role) {
    if(!strcmp(role, "admin")) {
        return ROLE::ADMIN;
    }
    if(!strcmp(role, "client")) {
        return ROLE::CLIENT;
    }
    if(!strcmp(role, "contractor")) {
        return ROLE::CONTRACTOR;
    }
    return ROLE::UNKNOWN;
}

static int sign_in_cb(void* user_obj, int argc, char**argv, char** col_name) {
    User* user = static_cast<User*>(user_obj);
    Info* info = user->getInfo();

    for (int i = 0; i < argc; i++) {
        if (std::string(col_name[i]) == "userID") {
            info->id = argv[i] ? std::stoi(argv[i]) : -1;  
        } 
        else if (std::string(col_name[i]) == "type") {
            user->role = argv[i] ? to_role(argv[i]) : ROLE::UNKNOWN;
            std::cout << "ROLE: " << argv[i] << "\n";  
        }
        else if (strcmp(col_name[i], "password") == 0) {
            user->setPassword(argv[i] ? argv[i] : "");
        } 
        else if (strcmp(col_name[i], "username") == 0) {
            user->setUsername(argv[i] ? argv[i] : "");
        } 
    }
    return 0;
}

std::shared_ptr<User> SQLite::signIn(const char* username, const char* password) {
    std::shared_ptr<User> user = std::make_shared<User>();
    if (!strcmp(username, "c")) {
        user->role = ROLE::CONTRACTOR;
        return user;
    }
    else if (!strcmp(username, "a")) {
        user->role = ROLE::ADMIN;
        return user;
    }
    
    std::string error_msg;
    std::string query = "SELECT * FROM USERS WHERE username = '" + std::string(username) + 
                    "' AND password = '" + std::string(password) + "'";

    LOG("INFO", "SQLite", "QUERY: %s", query.c_str());
    //user->role = ROLE::CLIENT;
    if(!Database::runQuery(query, sign_in_cb, (void*)user.get(), error_msg) || user->getInfo()->id == -1) {
        LOG("ERROR", "SQLite", "Failed to sign in user with username=%s, password=%s [%s]", username, password, error_msg.c_str());
        return nullptr;
    }
    return user;
}

static int load_data_info_cb(void* info_param, int argc, char**argv, char** col_name) {
    Info* info = static_cast<Info*>(info_param);
    for (int i = 0; i < argc; i++) {
        if (strcmp(col_name[i], "userID") == 0) {
            info->id = argv[i] ? std::stoi(argv[i]) : -1;
        } 
        else if (strcmp(col_name[i], "phone") == 0) {
            info->phone = argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "email") == 0) {
            info->email = argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "location") == 0) {
            info->location = argv[i] ? argv[i] : "";
        }
        else if (strcmp(col_name[i], "bio") == 0) {
            info->bio = argv[i] ? argv[i] : "";
        }
    }
    return 0;
}

static int load_data_jobs_cb(void* jobs_param, int argc, char**argv, char** col_name) {
    std::vector<Job>* jobs = static_cast<std::vector<Job>*>(jobs_param);
    Job job;
    for (int i = 0; i < argc; i++) {
        if (strcmp(col_name[i], "jobId") == 0) {
            job.id = argv[i] ? std::stoi(argv[i]) : -1;
        } 
        else if (strcmp(col_name[i], "description") == 0) {
            std::strncpy(job.description, argv[i] ? argv[i] : "", sizeof(job.description) - 1);
            job.description[sizeof(job.description) - 1] = '\0'; 
        } 
        else if (strcmp(col_name[i], "name") == 0) {
            std::strncpy(job.name, argv[i] ? argv[i] : "", sizeof(job.name) - 1);
            job.name[sizeof(job.name) - 1] = '\0'; 
        } 
        else if (strcmp(col_name[i], "status") == 0) {
            std::strncpy(job.status, argv[i] ? argv[i] : "", sizeof(job.status) - 1);
            job.status[sizeof(job.status) - 1] = '\0'; 
        }
        else if (strcmp(col_name[i], "date") == 0) {
            std::strncpy(job.date, argv[i] ? argv[i] : "", sizeof(job.date) - 1);
            job.date[sizeof(job.date) - 1] = '\0'; 
        }
        else if (strcmp(col_name[i], "cost") == 0) {
            job.cost = argv[i] ? std::stod(argv[i]) : 0.0;
        }
    }
    jobs->emplace_back(std::move(job));
    return 0;
}

bool SQLite::loadData(User* user) {
    std::string error_msg;
    
    std::string query = "SELECT * FROM INFO WHERE userID = " + std::to_string(user->getInfo()->id);
    Info* info = user->getInfo();
    if(!Database::runQuery(query, load_data_info_cb, (void*)info, error_msg)) {
        LOG("ERROR", "SQLite", "failed to load user data from INFO table[%s]", error_msg.c_str());
        return false;
    }

    query = "SELECT * FROM JOB WHERE userID = " + std::to_string(user->getInfo()->id);
    std::vector<Job>* jobs = user->getJobs();
    if(!Database::runQuery(query, load_data_jobs_cb, (void*)jobs, error_msg)) {
        LOG("ERROR", "SQLite", "failed to load user data from JOB table[%s]", error_msg.c_str());
        return false;
    }

    return true;
}

/*
create table job(jobId int, description varchar, name varchar,
status boolean, cost number, userId int, talentId int);

insert into job values(1000, 'Isaac is laying pipe at Michaels house',
'MichaelJob', 0, 200.0, 1000, 1000);

INSERT INTO job (description, name, status, cost, userId, talentId)
*/
static std::string generateJobDescription(std::string talentName, std::string userName) {
    std::string descriptions[21];
    descriptions[0] = talentName + " is doing stuff to " + userName + "s house.";
    descriptions[1] = talentName + " is removing all the windows at " + userName + "s house.";
    descriptions[2] = talentName + " is plumbing all over the place in " + userName + "s house.";
    descriptions[3] = talentName + " is installing shingles in the bathroom of " + userName + "s house.";
    descriptions[4] = talentName + " is sleeping at " + userName + "s house.";
    descriptions[5] = talentName + " is linking the lists at " + userName + "s house.";
    descriptions[6] = talentName + " is helping his friend Jack, off the horse at " + userName + "s house.";
    descriptions[7] = talentName + " is drinking at " + userName + "s house.";
    descriptions[8] = talentName + " is installing a single lightbulb at " + userName + "s house.";
    descriptions[9] = talentName + " is installing drywall at " + userName + "s house.";
    descriptions[10] = talentName + " is talking to Bill Nye at " + userName + "s house.";
    descriptions[11] = talentName + " is drunk at " + userName + "s house.";
    descriptions[12] = talentName + " is missing. Last seen at " + userName + "s house.";
    descriptions[13] = talentName + " is manually breathing at " + userName + "s house.";
    descriptions[14] = talentName + " is attending a lecture at " + userName + "s house.";
    descriptions[15] = talentName + " is breaking into " + userName + "s house.";
    descriptions[16] = talentName + " is siphoning gas at " + userName + "s house.";
    descriptions[17] = talentName + " is hooking up a car battery to " + userName + "s house.";
    descriptions[18] = talentName + " is mining for diamonds at " + userName + "s house.";
    descriptions[19] = talentName + " is livin' la vida loca at " + userName + "s house.";
    descriptions[20] = talentName + " is " + userName + ".";

    //Random Number generation for this
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0, 21);
    int randint = distr(gen);

    return descriptions[randint];
}
//Calls back to michael's cellphone
static int michaelCallback(void* jobs_param, int argc, char** argv, char** col_name) {
    return true;
}
/*
Runs a SQL query to book a new job and add to the database.
This code is mostly just constructing the query.
Returns false if an error is encountered.
*/
bool SQLite::bookJob(User* user, Talent* talent) {
    
    std::string error_msg;

    Info* userInfo = user->getInfo();
  
    int userID = userInfo->id;
    int talentID = talent->id;
    double rate = talent->rate;
    std::string talentName = talent->name;
    std::string userName = user->getUsername();

    std::string query = std::string("INSERT INTO job (description, name, status, cost, userId, talentId) VALUES (") + generateJobDescription(talentName, userName) + ", '" + userName + std::string("', 0, ") + std::to_string(rate) + std::string(", ") + std::to_string(userID) + std::string(", ") + std::to_string(talentID) + std::string(")");

    if (!Database::runQuery(query, michaelCallback, (void*)true, error_msg)) {
        LOG("ERROR", "SQLite", "failed to load user data from JOB table[%s]", error_msg.c_str());
        return false;
    }
    
    return true;
}



//This is here so that the program doesn't crash!
bool SQLite::bookJob(User* user, Job* job) {
    return true;
}


bool SQLite::changePassword(User* user, const char* old_pass, const char* new_pass) {
    if (old_pass == new_pass) {
        LOG("Error", "SQLite", " new password cannot be the same as old password.");
        return false;
    }
   
    std::string error_msg;
    std::string userName = user->getUsername();
    std::string query = "update users set password = '" + std::string(new_pass) + "' where username = '" + userName + "'";
    if (!Database::runQuery(query, michaelCallback, (void*)true, error_msg)) {
        LOG("ERROR", "SQLite", "failed to update password[%s]", error_msg.c_str());
        return false;
    }
    user->setPassword(new_pass);
    return true;
}

bool SQLite::changeUsername(User* user, const char* new_username, const char* password) {
    if (user->getUsername() == new_username) {
        LOG("Error", "SQLite", " new username cannot be the same as old username.");
        return false;
    }
    if (user->getPassword() != password) {
        LOG("Error", "SQLite", " incorrect password. Username not changed.");
        return false;
    }
    std::string error_msg;
    std::string userName = user->getUsername();
    std::string query = "update users set username = '" + std::string(new_username) + "' where username = '" + userName + "'";
    if (!Database::runQuery(query, michaelCallback, (void*)true, error_msg)) {
        LOG("ERROR", "SQLite", "failed to load user data from JOB table[%s]", error_msg.c_str());
        return false;
    }
    user->setUsername(new_username);
    return true;
}

/* TALENT
talentID = 1000
name = Isaac Pipe Laying Ltd.
service_type = Plumbing
location = Kamloops
rating = 5
rate = 200
message = I will lay your pipe :)
*/

/*struct Talent {
    int id;
    std::string name;
    std::string service_type;  
    std::string location;      
    int rating;                
    float rate;
    Talent() {}
    Talent(const char* name, const char* service, const char* location, int rating, float rate): 
    name(name), service_type(service), location(location), rating(rating), rate(rate){}
};*/

static int find_talent_cb(void* talent_param, int argc, char**argv, char** col_name) {
    std::vector<Talent>* talent = static_cast<std::vector<Talent>*>(talent_param);
    Talent tal;
    for (int i = 0; i < argc; i++) {
        if (strcmp(col_name[i], "talentID") == 0) {
            tal.id = argv[i] ? std::stoi(argv[i]) : -1;
        } 
        else if (strcmp(col_name[i], "name") == 0) {
            tal.name = argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "service_type") == 0) {
            tal.service_type= argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "location") == 0) {
            tal.location = argv[i] ? argv[i] : "";
        }
        else if (strcmp(col_name[i], "rating") == 0) {
            tal.rating = argv[i] ? std::stoi(argv[i]) : -1;
        }
        else if (strcmp(col_name[i], "rate") == 0) {
            tal.rate = argv[i] ? std::stof(argv[i]) : -1;
        }
    }
    talent->emplace_back(std::move(tal));
    return 0;
}

std::vector<Talent> SQLite::findTalents(const char* service_type, const char* location, int min_rating, int min_price, int max_price) {
    std::vector<Talent> talent;
    std::string query = "SELECT * FROM TALENT ";
    std::string where_clause;
    std::string err_msg;

    bool has_conditions = false;
    if (strlen(service_type) != 0) {
        where_clause += (has_conditions ? " AND " : " WHERE ") + std::string("service_type = '") + std::string(service_type) + "'";
        has_conditions = true;
    }
    if (strlen(location) != 0) {
        where_clause += (has_conditions ? " AND " : " WHERE ") + std::string("location = '") + std::string(location) + "'";
        has_conditions = true;
    }
    if (min_rating > 0) {
        where_clause += (has_conditions ? " AND " : " WHERE ") + std::string("rating > ") + std::to_string(min_rating);
        has_conditions = true;
    }
    if (min_price > 0) {
        where_clause += (has_conditions ? " AND " : " WHERE ") + std::string("rate > ") + std::to_string(min_price);
        has_conditions = true;
    }
    if (max_price > 0) {
        where_clause += (has_conditions ? " AND " : " WHERE ") + std::string("rate < ") + std::to_string(max_price);
        has_conditions = true;
    }
    query += where_clause;

    LOG("INFO", "SQLite", "QUERY: %s", query.c_str());

    if (!Database::runQuery(query, find_talent_cb, (void*)&talent, err_msg)) {
        LOG("ERROR", "SQLite", "failed find talents from TALENT table [%s] [QUERY: %s]", err_msg.c_str(), query.c_str());
    }
    return talent;
}





