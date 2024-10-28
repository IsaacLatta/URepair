#include "View.h"
#include "App.h"

void LoginView::handle() {
    static const std::size_t BUFSIZE = 256;
    static char username_buffer[BUFSIZE];
	static char password_buffer[BUFSIZE];
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(10.0f, 10.0f);  // Increase padding inside input fields and buttons
    style.ItemSpacing = ImVec2(10.0f, 20.0f);   // Increase space between items

    // Input fields and buttons with larger sizes
    ImGui::Begin("Welcome to URepair", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Username", username_buffer, BUFSIZE);
    
    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Password", password_buffer, BUFSIZE, ImGuiInputTextFlags_Password);
    
    if (ImGui::Button("Login", ImVec2(200, 50))) {  
        if(loginHandler) 
            loginHandler(username_buffer, password_buffer);
        else
            ERROR("LoginView", "no login handler");
        memset(username_buffer, '\0', BUFSIZE);
        memset(password_buffer, '\0', BUFSIZE);
    }

    ImGui::End();
}

