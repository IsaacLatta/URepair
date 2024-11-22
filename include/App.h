#ifndef APP_H
#define APP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "logger.h"
#include "Controller.h"

#define DEFAULT_HEIGHT 1000
#define DEFAULT_WIDTH 1000

class Controller;
class Database;
class User;

/**
 * @class App
 * @brief Initializes and manages the main application loop, including rendering and input handling.
 *
 * The App class is responsible for setting up and managing the OpenGL context, the ImGui interface,
 * and the application's main loop. It serves as the entry point for the program and integrates 
 * the Controller class to manage the application's state and views.
 */
class App {
public:
    /**
     * @brief Constructs an App instance.
     *
     * Initializes member variables such as the GLFW window and the controller.
     */
    App();

    /**
     * @brief Destroys the App instance and cleans up allocated resources.
     *
     * Deletes the controller and ensures proper cleanup of all resources.
     */
    ~App();

    /**
     * @brief Initializes the application.
     *
     * Sets up GLFW and GLAD for OpenGL, initializes ImGui, and starts the Controller.
     *
     * @return true if initialization is successful, false otherwise.
     */
    bool init();

    /**
     * @brief Runs the application's main loop.
     *
     * Starts the event polling and rendering loop, and ensures cleanup when the application exits.
     */
    void run();

private:
    /**
     * @brief The main rendering and event loop for the application.
     *
     * Handles event polling, updates the UI via ImGui, and manages the application's rendering process.
     */
    void mainLoop();

private:
    Controller* controller; ///< Manages application state and views.
    GLFWwindow* main_window; ///< Main application window created using GLFW.
};

#endif
