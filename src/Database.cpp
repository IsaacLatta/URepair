#include "Database.h"

Database* Database::databaseFactory() {
    INFO("database", "created");
    return new Dummy();
}

bool Dummy::connect() {
    return true;
}

User* Dummy::validate(User* user) {
   INFO("User", "validated");
   user->isLoggedIn = true;
   return new User(*user);
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

void Dummy::updateJobs(User* user) {
    std::vector<Job> jobs = {
        { 1, "Fix leaky faucet", "Plumber John", "Pending", "2024-10-25", 100.0 },
        { 2, "Electrical wiring", "Electrician Jane", "Completed", "2024-10-22", 250.0 },
        { 3, "Painting living room", "Painter Bob", "In Progress", "2024-10-24", 300.0 },
        { 4, "Install new sink", "Plumber Alice", "Pending", "2024-10-26", 150.0 },
        { 5, "Fix bathroom light", "Electrician Jane", "Completed", "2024-10-20", 200.0 }
    };
    user->setJobs(jobs);
}

bool Dummy::loadData(User* user) { 
    updateJobs(user);
    return true;
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
    