#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <stack>

#include "View.h"
#include "User.h"
#include "logger.h"
#include "Database.h"
#include "ThreadPool.h"

/**
 * @class Controller
 * @brief Manages application state and view transitions.
 *
 * The Controller class is responsible for managing the flow of the application,
 * connecting to the database, and handling transitions between different views.
 * It provides mechanisms to navigate forward and backward through the view history
 * and setups up the appropriate views based on the user's role.
 */
class Controller
{
public:
    /**
     * @brief Constructs a Controller instance.
     *
     * Initializes the Controller and sets up the necessary components.
     */
    Controller();

    /**
     * @brief Starts the application by connecting to the database and setting up the initial view.
     * 
     * @return true if the application starts successfully, false otherwise.
     */
    bool start();

    /**
     * @brief Renders the current view in the application.
     * 
     * Displays the top view in the history stack. If no view is available, logs an error.
     */
    void renderCurrent();
    
private:
    /**
     * @brief Sets up and displays the login view.
     *
     * The login view allows users to log in and initializes the main view after successful login.
     */
    void setupLoginView();

    /**
     * @brief Sets up the main view for the user based on their role.
     * 
     * @param user A shared pointer to the logged-in user.
     */
    void setupMainView(std::shared_ptr<User> user);

    /**
     * @brief Sets up the profile view for the current user.
     *
     * Depending on the user's role, the appropriate profile view is created and displayed.
     * 
     * @param user A shared pointer to the user whose profile is to be displayed.
     */
    void setupProfileView(std::shared_ptr<User> user);

    /**
     * @brief Sets up the main view for a client user.
     * 
     * @param client A shared pointer to the client user.
     */
    void setupMainClientView(std::shared_ptr<User> client);

    /**
     * @brief Sets up the main view for a contractor user.
     * 
     * @param contr A shared pointer to the contractor user.
     */
    void setupMainContractorView(std::shared_ptr<User> contr);

    /**
     * @brief Pushes a new view onto the history stack.
     * 
     * @param view A shared pointer to the view to be pushed.
     */
    void pushView(std::shared_ptr<View> view);

    /**
     * @brief Navigates back to the previous view in the history stack.
     *
     * Removes the current view from the history stack and moves it to the revisit stack.
     */
    void goBack();

    /**
     * @brief Navigates forward to the next view in the revisit stack.
     *
     * Moves the top view from the revisit stack back to the history stack.
     */
    void goForward();

private:
    std::shared_ptr<User> user; ///< Currently logged-in user.
    std::unique_ptr<Database> db; ///< Database instance for managing data.
    std::stack<std::shared_ptr<View>> history; ///< Stack of previously visited views.
    std::stack<std::shared_ptr<View>> revisit; ///< Stack of views available for forward navigation.
};


#endif