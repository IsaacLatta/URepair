#ifndef APP_H
#define APP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "logger.h"
#include "AppState.h"
#define DEFAULT_HEIGHT 1000
#define DEFAULT_WIDTH 1000

class App {
	public:
	App();
	~App();
	bool init();
	void run();
	void setNewState(AppState* new_state);
	void changeUser(User* user);
	User* getUser();
	private:
	void changeState();
	void mainLoop();
	User* user;
	AppState* state;
	AppState* next_state;
	GLFWwindow* main_window;
};


#endif
