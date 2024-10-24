#include "AppState.h"
#include "app.h"


void LoginState::handle() {
    User* user = app->getUser();
    char username_buffer[128];
	char password_buffer[128];
	strncpy(username_buffer, user->getUsername().c_str(), sizeof(username_buffer));
	strncpy(password_buffer, user->getPassword().c_str(), sizeof(password_buffer));

    // Make the whole UI larger with global scaling
    ImGui::GetIO().FontGlobalScale = 1.25f;  // Scale the entire UI by 1.25x

    // Increase padding and spacing
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(10.0f, 10.0f);  // Increase padding inside input fields and buttons
    style.ItemSpacing = ImVec2(10.0f, 20.0f);   // Increase space between items

    // Input fields and buttons with larger sizes
    ImGui::Begin("Welcome to URepair");

    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Username", username_buffer, IM_ARRAYSIZE(username_buffer));
    user->setUsername(username_buffer);

    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Password", password_buffer, IM_ARRAYSIZE(password_buffer), ImGuiInputTextFlags_Password);
    user->setPassword(password_buffer);

    if (ImGui::Button("Login", ImVec2(200, 50))) {  
        INFO("login username", username_buffer);
        INFO("login password", password_buffer);
        user->isLoggedIn = true;
        app->changeState(new MainState(app));
    }

    ImGui::End();
}

