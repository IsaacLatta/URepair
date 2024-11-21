#include "Database.h"

bool SQLite::connect() {
    return true;
}

bool run_query(const char* query, void* data) {
    return true;
}

static int loaddata_callback(void* user_obj, int argc, char**argv, char** col_name) {
    User* user = static_cast<User*>(user_obj);


    return 0;

}

bool SQLite::loadData(User* user) {
    return true;
}