#include "View.h"
#include "App.h"

void ProfileView::showEditOptions(bool* p_open) {
    if(!*p_open) {
        return;
    }
    
    static char name[BUFFER_SIZE] = "\0";
    static char phone[BUFFER_SIZE] = "\0";
    static char email[BUFFER_SIZE] = "\0";
    static char location[BUFFER_SIZE] = "\0";
    
    // Edit options inputs
    ImGui::Text("New name");
    ImGui::SameLine();
    ImGui::InputText("##name", name, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##1")) {

    }
    ImGui::Text("New phone");
    ImGui::SameLine();
    ImGui::InputText("##phone", phone, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##2")) {

    }
    ImGui::Text("New email");
    ImGui::SameLine();
    ImGui::InputText("##email", email, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##3")) {

    }
    ImGui::Text("New location");
    ImGui::SameLine();
    ImGui::InputText("##location", location, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##4")) {

    }
    ImGui::Text("New bio");
    ImGui::SameLine();
    ImGui::InputTextMultiline("##bio", location, BUFFER_SIZE, ImVec2(400, 200));
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##5")) {

    }
    if (ImGui::Button("Cancel All")) {
        memset(name, '\0', BUFFER_SIZE);
        memset(phone, '\0', BUFFER_SIZE);
        memset(email, '\0', BUFFER_SIZE);
        memset(location, '\0', BUFFER_SIZE);
        *p_open = false;
    }
}

void ProfileView::showProfileInfo() {

    static bool edit = false;
    Info* info = user->getInfo();
    ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_Always);
    ImGui::Begin("Client Profile", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {
        edit = true;
    }

    ImGui::Text(info->name.c_str());
    ImGui::Separator();

    // Client Info
    ImGui::Text("Name: %s", info->name.c_str());
    ImGui::Text("Contact: %s", info->phone.c_str());
    ImGui::Text("Email: %s", info->email.c_str());
    ImGui::Text("Location: %s", info->location.c_str());
    ImGui::Separator();

    // Bio
    ImGui::Text("Bio:");
    ImGui::TextWrapped("%s", info->bio.c_str());
    ImGui::Separator();
    showEditOptions(&edit);
    ImGui::End();
}

void ProfileView::showSecurityMenu(bool* p_open)
{
    if (!*p_open) {
        return;
    }
    static char username_buffer[BUFFER_SIZE], password_buffer1[BUFFER_SIZE], password_buffer2[BUFFER_SIZE];
    static bool change_username = false, change_password = false;
    if (ImGui::BeginMenu("Options")) {
        if (ImGui::MenuItem("Change password")) {
            change_password = !change_password;
        }
        if (ImGui::MenuItem("Change username")) {
            change_username = !change_username;
            ImGui::InputText("New username", username_buffer, BUFFER_SIZE);
        }
        ImGui::EndMenu();
    }
    if (change_password) {
        ImGui::SetNextItemWidth(300.0f);
        ImGui::Begin("Change Password", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::InputText("New password", password_buffer1, BUFFER_SIZE);
        ImGui::InputText("Enter again to confirm", password_buffer2, BUFFER_SIZE);
        if(ImGui::Button("Submit")) {
            if(!changePassword(password_buffer1, password_buffer2)) {
                ERROR("password change failed", "");
            }
            change_password = false;
            memset(password_buffer1, '\0', BUFFER_SIZE);
            memset(password_buffer2, '\0', BUFFER_SIZE);
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            change_password = false;
            memset(password_buffer1, '\0', BUFFER_SIZE);
            memset(password_buffer2, '\0', BUFFER_SIZE);
        }
        ImGui::End();
    }
    if (change_username) {
        ImGui::SetNextItemWidth(300.0f);
        ImGui::Begin("Change Username", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::InputText("New username", username_buffer, BUFFER_SIZE);
        ImGui::InputText("Password", password_buffer1, BUFFER_SIZE);
        if(ImGui::Button("Submit")) {
            if(!changeUsername(username_buffer, password_buffer1)) {
                ERROR("username change failed", "");
            }
            change_username = false;
            memset(password_buffer1, '\0', BUFFER_SIZE);
            memset(username_buffer, '\0', BUFFER_SIZE);
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            change_username = false;
            memset(password_buffer1, '\0', BUFFER_SIZE);
            memset(username_buffer, '\0', BUFFER_SIZE);
        }
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
        logoutHandler();
    }
    if(switch_to_main) {
        goBack();
    }
}

void ProfileView::handle()
{
    showOptions();
    showProfileInfo();
}
