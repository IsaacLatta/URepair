#ifndef APP_H
#define APP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "logger.h"

#define DEFAULT_HEIGHT 800
#define DEFAULT_WIDTH 600

class App {
	public:
	App();
	bool init();
	void run();
	private:
	
	void mainLoop();
	GLFWwindow* main_window;	
};


#endif
