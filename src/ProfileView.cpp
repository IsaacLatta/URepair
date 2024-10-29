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
            if(!changePassword(password_buffer1, password_buffer2)) {
                ERROR("password change failed", "");
            }
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
            if(!changeUsername(username_buffer, password_buffer1)) {
                ERROR("username change failed", "");
            }
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
    }
}

void ProfileView::ShowProfilePage() {
    std::vector<Job>* jobs = user->getJobs();
    ImGui::Begin("Client Profile");

    // Title
    ImGui::Text("Client Profile");
    ImGui::Separator();

    // Profile Picture Placeholder
    ImGui::Text("[ Profile Picture ]");
    ImGui::Separator();

    // Client Info
    ImGui::Text("Name: %s", user->getUsername().c_str());
   //ImGui::Text("Contact: %s", client.phone.c_str());
    //ImGui::Text("Email: %s", client.email.c_str());
    ImGui::Text("Location: %s", client.location.c_str());
    ImGui::Separator();

    // Bio
    ImGui::Text("Bio:");
    ImGui::TextWrapped("%s", client.bio.c_str());
    ImGui::Separator();

    // Previous Bookings
    ImGui::Text("Previous Bookings:");
    for (const auto& booking : client.previousBookings) {
        ImGui::BulletText("%s", booking.c_str());
    }
    ImGui::Separator();

    // Buttons
    if (ImGui::Button("Edit Profile")) {
        // Open edit profile modal or page
    }
    ImGui::SameLine();
    if (ImGui::Button("View Past Projects")) {
        // Show past projects or bookings
    }

    ImGui::End();
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
}
