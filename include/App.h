#ifndef APP_H
#define APP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "logger.h"
#include "View.h"
#include "Database.h"
#define DEFAULT_HEIGHT 1000
#define DEFAULT_WIDTH 1000

class App {
	public:
	App();
	~App();
	bool init();
	void run();
	void setNewState(View* new_state);
	void changeUser(User* user);
	User* getUser();
	Database* getDB() { return db; }
	private:
	void changeState();
	void mainLoop();
	Database* db;
	User* user;
	View* state;
	View* next_state;
	GLFWwindow* main_window;
};


#endif
