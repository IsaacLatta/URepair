
#include "Database.h"


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
    user->role = ROLE::CLIENT;
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

/* TALENT
talentID = 1000
name = Isaac Pipe Laying Ltd.
service_type = Plumbing
location = Kamloops
rating = 5
rate = 200
message = I will lay your pipe :)
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
};
*/

bool SQLite::bookJob(User*, Job*) {
    
    
    return true;
}

void SQLite::updateJobs(User*) {

}

bool SQLite::changePassword(User* user, const char* old_pass, const char* new_pass) {
    return true;
}

bool SQLite::changeUsername(User* user, const char* password, const char* new_username) {
    return true;
}

std::vector<Talent> SQLite::findTalents(const char* service_type, const char* location, int min_rating, int min_price, int max_price) {
    return std::vector<Talent>();
}


