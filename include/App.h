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

class App {
	public:
	App();
	~App();
	
	/*Initialize the application, setup the glfw and glad openGL loaders, initialize the controller*/
	bool init();
	
	/* Run the applications main loop, cleanup resources after exiting */
	void run();
	
	private:
	void mainLoop();
	
	private:
	Controller* controller;
	GLFWwindow* main_window;
};


#endif
