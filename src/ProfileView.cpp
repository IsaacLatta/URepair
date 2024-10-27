#include "View.h"
#include "App.h"

void ProfileView::showSecurityMenu(bool* p_open)
{
    if (!*p_open) {
        return;
    }
    static const std::size_t BUFSIZE = 256;
    static char username_buffer[BUFSIZE], password_buffer1[BUFSIZE], password_buffer2[BUFSIZE];
    static bool change_username = false, change_password = false;
    if (ImGui::BeginMenu("Options")) {
        if (ImGui::MenuItem("Change password")) {
            change_password = !change_password;
        }
        if (ImGui::MenuItem("Change username")) {
            change_username = !change_username;
            ImGui::InputText("New username", username_buffer, BUFSIZE);
        }
        ImGui::EndMenu();
    }
    if (change_password) {
        ImGui::SetNextItemWidth(300.0f);
        ImGui::Begin("Change Password", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::InputText("New password", password_buffer1, BUFSIZE);
        ImGui::InputText("Enter again to confirm", password_buffer2, BUFSIZE);
        if(ImGui::Button("Submit")) {
            app->getDB()->changePassword(app->getUser(), password_buffer1, password_buffer2);
            change_password = false;
            memset(password_buffer1, '\0', BUFSIZE);
            memset(password_buffer2, '\0', BUFSIZE);
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            change_password = false;
            memset(password_buffer1, '\0', BUFSIZE);
            memset(password_buffer2, '\0', BUFSIZE);
        }
        ImGui::End();
    }
    if (change_username) {
        ImGui::SetNextItemWidth(300.0f);
        ImGui::Begin("Change Username", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::InputText("New username", username_buffer, BUFSIZE);
        ImGui::InputText("Password", password_buffer1, BUFSIZE);
        if(ImGui::Button("Submit")) {
            app->getDB()->changeUsername(app->getUser(), username_buffer, password_buffer1);
            change_username = false;
            memset(password_buffer1, '\0', BUFSIZE);
            memset(username_buffer, '\0', BUFSIZE);
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            change_username = false;
            memset(password_buffer1, '\0', BUFSIZE);
            memset(username_buffer, '\0', BUFSIZE);
        }
        ImGui::End();
        ImGui::End();
    }
}

void ProfileView::showOptions() {
    static bool settings_open = true; // Control visibility of the settings menu
    bool logout = false;
    bool switch_to_main = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Security"))
        {
            showSecurityMenu(&settings_open); // Pass settings_open to control the submenu visibility
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Upload Document"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("FAQ")) { /* Handle FAQ action */ }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Logout")) {
            logout = true;
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Back")) {
            switch_to_main = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if(logout) {
        INFO("profile state", "switching to login state");
        app->setNewState(new LoginView(app));
    }
    if(switch_to_main) {
        INFO("profile state", "switching to main state");
        app->setNewState(new MainClientView(app));
    }
}

void ProfileView::handle()
{
    showOptions();
}
