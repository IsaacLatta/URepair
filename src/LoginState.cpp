#include "AppState.h"
#include "app.h"

void LoginState::handle() {
    User* user = app->getUser();
    char username_buffer[128];
	char password_buffer[128];
	strncpy(username_buffer, user->getUsername().c_str(), sizeof(username_buffer));
	strncpy(password_buffer, user->getPassword().c_str(), sizeof(password_buffer));

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