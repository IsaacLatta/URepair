#include "Database.h"


bool Dummy::bookJob(User* user, Talent* talent) {
    return true;
}
std::unique_ptr<Database> Database::databaseFactory() {
    INFO("database", "created");
    //return std::make_unique<Dummy>();
    return std::make_unique<SQLite>();
}



bool Database::runQuery(const std::string& query, int(*callback)(void*,int,char**,char**), void* callback_param, std::string& error_msg) {
    sqlite3* db;
    int ret_code;
    char* errmsg = nullptr;

    if((ret_code =  sqlite3_open(SQLITE_FILE, &db)) != SQLITE_OK) {
        error_msg = "cannot open Database: " + std::string(SQLITE_FILE) + " code=" + std::to_string(ret_code) + " (" + sqlite3_errmsg(db) + ")";
        sqlite3_close(db);
        return false;
    }

    if((ret_code = sqlite3_exec(db, query.c_str(), callback, callback_param, &errmsg)) != SQLITE_OK) {
        error_msg = "query failed with code=" + std::to_string(ret_code);
        if(errmsg) {
            error_msg += " (" + std::string(errmsg) + ")";
            sqlite3_free(errmsg);
        } 
        else {
            error_msg = " (Unknown Error)";
        }  

        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool Dummy::connect() {
    return true;
}

bool Dummy::loadData(User* user) {
    user->setInfo(Info("jdoe@email.com", "2505791234", "John Doe","Vancouver, BC" , "I am a super nice guy, who will definitly pay you"));
    std::vector<Job> jobs;
    if(user->role == ROLE::CLIENT) {
        jobs = {
            {1, "Fix leaky faucet", "Plumber John", "Pending", "2024-10-25", 100.0},
            {2, "Electrical wiring", "Electrician Jane", "Completed", "2024-10-22", 250.0},
            {3, "Painting living room", "Painter Bob", "In Progress", "2024-10-24", 300.0},
            {4, "Install new sink", "Plumber Alice", "Pending", "2024-10-26", 150.0},
            {5, "Fix bathroom light", "Electrician Jane", "Completed", "2024-10-20", 200.0}
            };
    }
    else {
        jobs = {
            {1, "Fix leaky faucet", "John doe", "Pending", "2024-10-25", 100.0},
            {2, "Electrical wiring", "Matia smolcok", "Completed", "2024-10-22", 250.0},
            {3, "Painting living room", "Isaac beegcoc", "In Progress", "2024-10-24", 300.0},
            {4, "Install new sink", "Michael nocoq", "Pending", "2024-10-26", 150.0},
            {5, "Fix bathroom light", "Jane smith", "Completed", "2024-10-20", 200.0}
            };
    }
    user->setJobs(jobs);
    return true;
}

std::shared_ptr<User> Dummy::signIn(const char* username, const char* password) {
    auto user = std::make_shared<User>();
    if (!strcmp(username, "c")) {
        user->role = ROLE::CONTRACTOR;
    }
    else if (!strcmp(username, "a")) {
        user->role = ROLE::ADMIN;
    }
    else
        user->role = ROLE::CLIENT;
    loadData(user.get());
    return user;
}

static std::vector<Talent> filterTalents(const std::vector<Talent>& talents, const char* service_type, const char* location, int min_rating, int min_price, int max_price) {
    std::vector<Talent> results;
    for (const Talent& talent : talents) {
        bool matches_service = (std::strlen(service_type) == 0) || (talent.service_type == service_type);
        bool matches_location = (std::strlen(location) == 0) || (talent.location == location);
        bool matches_rating = (talent.rating >= min_rating);
        bool matches_price = (talent.rate >= min_price && talent.rate <= max_price);

        if (matches_service && matches_location && matches_rating && matches_price) {
            results.push_back(talent);
        }
    }
    return results;
}

std::vector<Talent> Dummy::findTalents(const char* service_type, const char* location, int min_rating, int min_price, int max_price) {
    std::string search_params = "\nService: " + std::string(service_type) + "\n" +
                                "Location: " + std::string(location) + "\n" + 
                                "Min rating: " + std::to_string(min_rating) + "\n" +
                                "Min price: " + std::to_string(min_price) + "\n" +
                                "Max price: " + std::to_string(max_price);
    std::vector<Talent> talents = {
        { "John Doe", "Plumber", "New York", 5, 100 },
        { "Jane Smith", "Electrician", "Los Angeles", 4, 200 },
        { "Alice Johnson", "Plumber", "Chicago", 3, 150 },
        { "Bob Lee", "Painter", "New York", 4, 300 },
        { "Charlie Brown", "Electrician", "San Francisco", 5, 250 }
    };
    return filterTalents(talents, service_type, location, min_rating, min_price, max_price);
}

bool Dummy::changePassword(User* user, const char* old_pass, const char* new_pass) {
    if(strcmp(user->getPassword().c_str(), old_pass)) {
        return false;
    }
    user->setPassword(new_pass);
    return true;
}
bool Dummy::changeUsername(User* user, const char* password, const char* new_username) {
    if(strcmp(user->getPassword().c_str(), password)) {
        return false;
    }
    user->setUsername(new_username);
    return true;
}

bool Dummy::bookJob(User* user, Job* job) {
    return true;
}

bool Dummy::changeInfo(User* user, const char*, const char*, Talent* talent) {
    return true;
}