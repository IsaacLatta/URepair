#ifndef VIEW_H
#define VIEW_H
#include <vector>

#include "User.h"
#include <memory>
#include <functional>

#define BUFFER_SIZE 512
#define QUERY_BUFFER 8192

/**
 * @class View
 * @brief Base class for all views in the application.
 *
 * The `View` class provides a base interface for rendering different parts
 * of the user interface. Each derived class represents a specific view,
 * such as login, client, admin, contractor, or profile views.
 */
class View 
{
    public:
    /**
     * @brief Default constructor for the `View` class.
     */
    View(): user(nullptr) {}

    /**
     * @brief Constructor to initialize the `View` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    View(std::shared_ptr<User> user): user(user) {};

    /**
     * @brief Pure virtual function to render the view.
     */
    virtual void render() = 0;

    /**
     * @brief Virtual destructor for the `View` class.
     */
    virtual ~View() = default;

    protected:
    std::shared_ptr<User> user; ///< User associated with the view.
};

/**
 * @class LoginView
 * @brief View for the login interface.
 *
 * Handles rendering the login form and triggering the login handler.
 */
class LoginView: public View
{   
    public:
    std::function<void(const char*, const char*)> loginHandler = nullptr; ///< Callback for login handling.

    /**
     * @brief Constructor for `LoginView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    LoginView(std::shared_ptr<User> user): View(user) {};

    /**
     * @brief Default constructor for `LoginView`.
     */
    LoginView() {}

    /**
     * @brief Renders the login view.
     */
    void render() override;
};

/**
 * @class MainClientView
 * @brief View for the main client interface.
 *
 * Displays job listings, search filters, and search results for talents.
 */
class MainClientView: public View {
    public:
    std::vector<Talent> search_results; ///< Search results for talents.
    std::function<void(const char*, const char*, int, int, int)> searchHandler; ///< Callback for handling talent searches.
    std::function<void()> logoutHandler; ///< Callback for handling logout.
    std::function<void()> profileHandler; ///< Callback for navigating to the profile view.
    std::function<void(Talent*)> bookingHandler; ///< Callback for handling bookings.
    std::function<void()> uploadHandler; ///< Callback for handling file uploads.

    /**
     * @brief Constructor for `MainClientView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    explicit MainClientView(std::shared_ptr<User> user) : View(user) {}

    /**
     * @brief Renders the main client view.
     */
    void render() override;

    private:
    char search_service_type[128] = ""; ///< Buffer for the service type search filter.
    char search_location[128] = ""; ///< Buffer for the location search filter.
    int min_rating = 0; ///< Minimum rating filter.
    int min_price = 0; ///< Minimum price filter.
    int max_price = 500; ///< Maximum price filter.

    void showMenuBar(); ///< Displays the client menu bar.
    void showJobs(); ///< Displays the client's job list.
    void showTalentSearchFilters(); ///< Displays the talent search filters.
    void showTalentSearchResults(); ///< Displays the talent search results.
    void showBookingMenu(Talent* talent, bool*); ///< Displays the booking menu.
    void showMessageBox(bool* p_open, Talent*); ///< Displays the message box.
};

/**
 * @class ProfileView
 * @brief View for displaying and editing user profiles.
 *
 * Supports both client and contractor-specific profile editing and security options.
 */
class ProfileView: public View {
    public:
    std::function<void(char*, char*)> updateTalentInfoHandler; ///< Callback for updating talent info.
    std::function<void(char*, char*)> updateInfoHandler; ///< Callback for updating user info.
    std::function<bool(const char*, const char*)> changeUsername; ///< Callback for changing the username.
    std::function<bool(const char*, const char*)> changePassword; ///< Callback for changing the password.
    std::function<void()> logoutHandler; ///< Callback for handling logout.
    std::function<void()> goBack; ///< Callback for navigating back to the previous view.

    /**
     * @brief Constructor for `ProfileView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    ProfileView(std::shared_ptr<User> user): View(user) {}

    /**
     * @brief Renders the profile view.
     */
    void render() override;

    protected:
    void showOptions(); ///< Displays the profile options menu.
    void showSecurityMenu(bool*); ///< Displays the security settings menu.
    void showProfileInfo(); ///< Displays client-specific profile information.
    void showEditOptions(bool*); ///< Displays the edit options for client profiles.
    void showContractorInfo(); ///< Displays contractor-specific profile information.
    void showContractorEditOptions(bool* stay_open); ///< Displays edit options for contractor profiles.
};

/**
 * @class MainAdminView
 * @brief View for the main admin interface.
 *
 * Allows the admin to run queries and manage users.
 */
class MainAdminView : public View 
{
    public:
    std::function<std::string(const char*)> queryHandler; ///< Callback for handling queries.
    std::function<void()> logoutHandler; ///< Callback for handling logout.

    /**
     * @brief Constructor for `MainAdminView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    MainAdminView(std::shared_ptr<User> user): View(user) {}

    private:
    /**
     * @brief Renders the main admin view.
     */
    void render() override;
};

/**
 * @class ClientProfileView
 * @brief Specialized view for displaying client profiles.
 */
class ClientProfileView: public ProfileView {
    public:
    /**
     * @brief Constructor for `ClientProfileView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    ClientProfileView(std::shared_ptr<User> user): ProfileView(user) {}

    /**
     * @brief Renders the client profile view.
     */
    void render() override;
};

/**
 * @class MainContractorView
 * @brief View for the main contractor interface.
 *
 * Displays active jobs, job requests, and contractor-specific menus.
 */
class MainContractorView: public View {
    public:
    /* Initialized to default logs */
    std::function<void(Job*, bool)> jobAcceptHandler = [](Job*, bool) { INFO("view", "would call accept job"); }; ///< Callback for handling job acceptance.
    std::function<void()> logoutHandler = []() { INFO("view", "would log user out"); }; ///< Callback for handling logout.
    std::function<void()> profileHandler = []() { INFO("view", "user can view profile"); }; ///< Callback for viewing the profile.
    std::function<void()> activejobHandler = []() { INFO("view", "see open jobs "); }; ///< Callback for viewing active jobs.

    /**
     * @brief Constructor for `MainContractorView` with a user.
     * @param user A shared pointer to the user object associated with the view.
     */
    explicit MainContractorView(std::shared_ptr<User> user) : View(user) {}

    /**
     * @brief Renders the main contractor view.
     */
    void render() override;

    private:
    void showContractorMenuBar(); ///< Displays the contractor menu bar.
    void showJobRequestInfo(Job* job, bool*); ///< Displays detailed job request information.
    void showJobRequests(); ///< Displays pending job requests.
    void showActiveJobs(); ///< Displays active jobs.
    void showAllJobs(bool* stay_open); ///< Displays all jobs.
};

#endif
