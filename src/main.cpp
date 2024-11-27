#include "App.h"
#include <sqlite3.h>
#include <iostream>

/**
 * @brief Entry point of the application.
 *
 * This function initializes the application, sets up necessary components,
 * and starts the main event loop.
 * 
 * The program flow is as follows:
 * 1. Create an instance of the `App` class.
 * 2. Initialize the application by setting up GLFW, OpenGL, and ImGui.
 * 3. If initialization fails, log the error and terminate the program.
 * 4. If initialization succeeds, enter the main application loop.
 * 
 * @return 0 on successful execution, or -1 if initialization fails.
 */
int main() {
    // Create an instance of the App class, which handles initialization and the main loop.
    App app;

    // Attempt to initialize the application components (GLFW, OpenGL, ImGui, etc.).
    if (!app.init()) {
        // Log an error message if initialization fails and exit with a non-zero status.
        ERROR("App", "init failed, exiting ...");
        return -1;
    }

    // If initialization is successful, run the main application loop.
    app.run();

    // Return 0 to indicate successful program execution.
    return 0;
}
