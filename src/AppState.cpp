#include "AppState.h"
#include "app.h"

void MainState::showMenu(bool* p_open) {
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin("Menu", p_open, ImGuiWindowFlags_MenuBar);

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Say yes")) {
			if(ImGui::MenuItem("Are you sure?")) {
				INFO("menu", "yes");
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Say No")) {
			if(ImGui::BeginMenu("Are you sure")) {
				if(ImGui::MenuItem("Yes")) {
					INFO("menu", "no");
				}
				if(ImGui::MenuItem("No")) {
					INFO("menu", "good call");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void MainState::showMainWindow() {
	ImGui::Begin("Hello!");
	ImGui::Text("This is a window.");
	if(ImGui::Button("Click Me")) {
		INFO("Button", "clicked");
	}
	ImGui::End();

	bool show_menu = true;
	showMenu(&show_menu);
}

void MainState::handle() {
    showMainWindow();
}

void LoginState::handle() {
    char username_buffer[128];
	char password_buffer[128];
    User* user = app->getUser();
	strncpy(username_buffer, user->getUsername().c_str(), sizeof(username_buffer));
	strncpy(password_buffer, user->getPassword().c_str(), sizeof(password_buffer));

	// Display the input fields with the buffers
	ImGui::Begin("Welcome to URepair");
	ImGui::InputText("Username", username_buffer, IM_ARRAYSIZE(username_buffer));
	user->setUsername(username_buffer);
	ImGui::InputText("Password", password_buffer, IM_ARRAYSIZE(password_buffer), ImGuiInputTextFlags_Password);
	user->setPassword(password_buffer);

	if(ImGui::Button("Login")) {
		INFO("login username", username_buffer);
		INFO("login password", password_buffer);
		user->isLoggedIn = true;
	}

    if(user->isLoggedIn) {
        app->changeState(new MainState(this->app));
    }

	ImGui::End();
}