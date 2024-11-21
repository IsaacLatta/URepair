#include "Database.h"

bool SQLite::connect() {
    return true;
}

bool run_query(const char* query, void* data) {
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