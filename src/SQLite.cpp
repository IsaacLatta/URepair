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

/* TALENT
talentID = 1000
name = Isaac Pipe Laying Ltd.
service_type = Plumbing
location = Kamloops
rating = 5
rate = 200
message = I will lay your pipe :)
*/

static int loaddata_callback(void* user_obj, int argc, char**argv, char** col_name) {
    User* user = static_cast<User*>(user_obj);



    return 0;

}

bool SQLite::loadData(User* user) {
    

    
    return true;
}

bool run_query(const std::string& query, int (*callback)(void*,int,char**,char**), void* callback_param, std::string& error_msg) {
    sqlite3* db;
    int ret_code;
    char* errmsg = "\0";

    if((ret_code =  sqlite3_open(SQLITE_FILE, &db)) != SQLITE_OK) {
        error_msg = "cannot open Database: " + std::string(SQLITE_FILE) + " code=" + std::to_string(ret_code) + " (" + sqlite3_errmsg(db) + ")";
        return false;
    }

    if(((ret_code) = sqlite3_exec(db, query.c_str(), callback, callback_param, &errmsg)) != SQLITE_OK) {
        error_msg = "query failed with code=" + std::to_string(ret_code) + " (" + std::string(errmsg) + ")";
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}
