
#include "Database.h"

bool SQLite::connect() {
    return true;
}


/* JOB
jobId = 1000
description = Isaac is laying pipe at Michaels house
name = MichaelJob
status = 0
cost = 200
userId = 1000
talentId = 1000
*/

/* INFO
userID = 1000
phone = 250-320-0439
email = mikeybleven@gmail.com
name = Michael
location = Kamloops
bio = I am a person
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
    }
    return 0;
}

std::shared_ptr<User> SQLite::signIn(const char* username, const char* password) {
    std::string error_msg;
    std::string query = "SELECT userID, type FROM USER WHERE username = " + std::string(username) 
                        + " AND password =" + std::string(password);
    
    auto user = std::make_shared<User>();
    if(!Database::runQuery(query, sign_in_cb, (void*)user.get(), error_msg) || user->getInfo()->id == -1) {
        LOG("ERROR", "SQLite", "Failed to sign in user with username=%s, password=%s [%s]", username, password, error_msg.c_str());
        return nullptr;
    }
    return user;
}

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

static int load_data_callback(void* user_obj, int argc, char**argv, char** col_name) {
    User* user = static_cast<User*>(user_obj);
    Info* info = user->getInfo();

    std::string fname, lname;
    for (int i = 0; i < argc; i++) {
        if (strcmp(col_name[i], "userID") == 0) {
            info->id = argv[i] ? std::stoi(argv[i]) : -1;
        } 
        else if (strcmp(col_name[i], "password") == 0) {
            user->setPassword(argv[i] ? argv[i] : "");
        } 
        else if (strcmp(col_name[i], "username") == 0) {
            user->setUsername(argv[i] ? argv[i] : "");
        } 
        else if (strcmp(col_name[i], "fname") == 0) {
            fname = argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "lname") == 0) {
            lname = argv[i] ? argv[i] : "";
        } 
        else if (strcmp(col_name[i], "type") == 0) {
            user->role = argv[i] ? to_role(argv[i]) : ROLE::UNKNOWN;
        }
    }
    info->name = fname + (fname.empty() || lname.empty() ? "" : " ") + lname;
    return 0;
}

bool SQLite::loadData(User* user) {
    std::string error_msg;
    std::string query = "SELECT * FROM USER WHERE userID = " + std::to_string((user->getInfo()->id));
    
    if(!Database::runQuery(query, load_data_callback, (void*)user, error_msg)) {
        LOG("ERROR", "SQLite", "failed to load data for user[%s]", error_msg.c_str());
        return false;
    }

    return true;
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


