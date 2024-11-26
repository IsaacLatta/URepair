#include "View.h"
#include "App.h"

void ProfileView::showEditOptions(bool* p_open) {
    if(!*p_open) {
        return;
    }

    char* field = nullptr;
    char new_field[BUFFER_SIZE];
    bool update = false;

    static char name[BUFFER_SIZE] = "\0";
    static char phone[BUFFER_SIZE] = "\0";
    static char email[BUFFER_SIZE] = "\0";
    static char location[BUFFER_SIZE] = "\0";
    static char bio[BUFFER_SIZE] = "\0";

    
    // Edit options inputs
    ImGui::Text("New name");
    ImGui::SameLine();
    ImGui::InputText("##name", name, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##1")) {
        update = true;
        field = "name\0";
        strncpy(new_field, name, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE-1] = '\0';
    }
    ImGui::Text("New phone");
    ImGui::SameLine();
    ImGui::InputText("##phone", phone, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##2")) {
        update = true;
        field = "phone\0";
        strncpy(new_field, phone, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE-1] = '\0';
    }
    ImGui::Text("New email");
    ImGui::SameLine();
    ImGui::InputText("##email", email, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##3")) {
        update = true;
        field = "email\0";
        strncpy(new_field, email, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE-1] = '\0';
    }
    ImGui::Text("New location");
    ImGui::SameLine();
    ImGui::InputText("##location", location, BUFFER_SIZE);
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##4")) {
        update = true;
        field = "location\0";
        strncpy(new_field, location, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE-1] = '\0';
    }
    ImGui::Text("New bio");
    ImGui::SameLine();
    ImGui::InputTextMultiline("##bio", location, BUFFER_SIZE, ImVec2(400, 200));
    ImGui::SameLine();
    if(ImGui::SmallButton("Save##5")) {
        update = true;
        field = "bio\0";
        strncpy(new_field, bio, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE-1] = '\0';
    }
    if (ImGui::Button("Cancel All")) {
        memset(name, '\0', BUFFER_SIZE);
        memset(phone, '\0', BUFFER_SIZE);
        memset(email, '\0', BUFFER_SIZE);
        memset(location, '\0', BUFFER_SIZE);
        *p_open = false;
    }
    
    if(update && updateInfoHandler) {
        updateInfoHandler(field, new_field);
        memset(name, '\0', BUFFER_SIZE);
        memset(phone, '\0', BUFFER_SIZE);
        memset(email, '\0', BUFFER_SIZE);
        memset(location, '\0', BUFFER_SIZE);
        *p_open = false;
    }
}

/*struct Talent {
    int id;                  ///< Unique identifier for the talent.
    std::string name;        ///< Name of the talent.
    std::string service_type;///< Type of service provided by the talent.
    std::string location;    ///< Location of the talent.
    int rating;              ///< Rating of the talent.
    float rate;              ///< Service rate of the talent.

    Talent() {}
    Talent(const char* name, const char* service, const char* location, int rating, float rate) :
    name(name), service_type(service), location(location), rating(rating), rate(rate){}
};*/

void ProfileView::showContractorEditOptions(bool* stay_open) {
    if (!*stay_open) {
        return;
    }

    char* field = nullptr;
    char new_field[BUFFER_SIZE] = "\0";
    bool update = false;

    static char name[BUFFER_SIZE] = "\0";
    static char service_type[BUFFER_SIZE] = "\0";
    static char location[BUFFER_SIZE] = "\0";
    static int rating = 0;     // Initialize rating as an int
    static float rate = 0.0f;  // Initialize rate as a float

    // Edit options inputs
    ImGui::Text("New Name");
    ImGui::SameLine();
    ImGui::InputText("##name", name, BUFFER_SIZE);
    ImGui::SameLine();
    if (ImGui::SmallButton("Save##name")) {
        update = true;
        field = "name";
        strncpy(new_field, name, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE - 1] = '\0';
    }

    ImGui::Text("New Service Type");
    ImGui::SameLine();
    ImGui::InputText("##service_type", service_type, BUFFER_SIZE);
    ImGui::SameLine();
    if (ImGui::SmallButton("Save##service_type")) {
        update = true;
        field = "service_type";
        strncpy(new_field, service_type, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE - 1] = '\0';
    }

    ImGui::Text("New Location");
    ImGui::SameLine();
    ImGui::InputText("##location", location, BUFFER_SIZE);
    ImGui::SameLine();
    if (ImGui::SmallButton("Save##location")) {
        update = true;
        field = "location";
        strncpy(new_field, location, BUFFER_SIZE - 1);
        new_field[BUFFER_SIZE - 1] = '\0';
    }

    ImGui::Text("New Rate");
    ImGui::SameLine();
    if (ImGui::InputFloat("##rate", &rate, 0.1f, 1.0f, "%.2f")) {
        update = true;
        field = "rate";
    }

    if (ImGui::Button("Cancel All")) {
        memset(name, '\0', BUFFER_SIZE);
        memset(service_type, '\0', BUFFER_SIZE);
        memset(location, '\0', BUFFER_SIZE);
        rating = 0;
        rate = 0.0f;
        *stay_open = false;
    }

    if (update && updateInfoHandler) {
        if (!strcmp(field, "rate")) {
            snprintf(new_field, BUFFER_SIZE, "%.2f", rate); // Convert float to string
        }
        updateTalentInfoHandler(field, new_field);

        memset(name, '\0', BUFFER_SIZE);
        memset(service_type, '\0', BUFFER_SIZE);
        memset(location, '\0', BUFFER_SIZE);
        rating = 0;
        rate = 0.0f;
        *stay_open = false;
    }
}

void ProfileView::showContractorInfo() {
    static bool edit = false;
    Talent* talent = user->getTalent();  

    //ImGui::Text("Contractor Profile");
    ImGui::Separator();

    ImGui::Text("Company: %s", talent->name.c_str());
    ImGui::Text("Service Type: %s", talent->service_type.c_str());
    ImGui::Text("Location: %s", talent->location.c_str());
    ImGui::Text("Rating: %d", talent->rating);
    ImGui::Text("Rate: $%.2f/hr", talent->rate);
    ImGui::Separator();

    if (ImGui::Button("Edit##1")) {
        edit = true;
    }

    showContractorEditOptions(&edit);
}


void ProfileView::showProfileInfo() {

    static bool edit = false;
    Info* info = user->getInfo();
    ImGui::SameLine();
    if (ImGui::Button("Edit##2")) {
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

    if(user->role == ROLE::CONTRACTOR) {
        showContractorInfo();
    }

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

void ProfileView::render()
{
    ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin("Profile", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
    showOptions();

    ImGui::BeginChild("Client Profile");
    showProfileInfo();
    ImGui::EndChild();

    ImGui::End();
}
