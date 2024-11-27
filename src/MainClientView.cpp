#include "View.h"
#include "App.h"

void MainClientView::showMenuBar() {
	static bool change_to_profile = false;
	static bool logout = false;
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Logout")) {
				logout = true;
			} 
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Profile")) {
            if(ImGui::MenuItem("View Profile")) {
				change_to_profile = true;
			}   
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
	if(change_to_profile) {
        change_to_profile = false;
		profileHandler();
	}
	if(logout) {
        logout = false;
		logoutHandler();
	}
}

void MainClientView::showJobs() {
	std::vector<Job>* jobs = user->getJobs();
	 if (ImGui::BeginTable("Jobs Table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("ID", 0, 50.0f);              // Fixed width 50
        ImGui::TableSetupColumn("Description", 0, 200.0f);    // Fixed width 200
        ImGui::TableSetupColumn("Name", 0, 150.0f);           // Fixed width 150
        ImGui::TableSetupColumn("Status", 0, 120.0f);         // Fixed width 120
        ImGui::TableSetupColumn("Date", 0, 150.0f);           // Fixed width 150
        ImGui::TableSetupColumn("Cost", 0, 100.0f);           // Fixed width 100
        ImGui::TableHeadersRow();

        for (const Job& job : *jobs) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", job.id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", job.description);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", job.name);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", job.status);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", job.date);
            ImGui::TableSetColumnIndex(5); ImGui::Text("$%.2f", job.cost);
        }

        ImGui::EndTable();
    }
}

void MainClientView::showTalentSearchFilters() {
    ImGui::Text("Find Talent:");

    ImGui::InputText("Service Type", search_service_type, IM_ARRAYSIZE(search_service_type));

    ImGui::InputText("Location", search_location, IM_ARRAYSIZE(search_location));
    ImGui::SliderInt("Min Rating", &min_rating, 0, 5);

    ImGui::SliderInt("Min Price", &min_price, 0, 500);
    ImGui::SliderInt("Max Price", &max_price, 0, 500);

    if (ImGui::Button("Search")) {
        searchHandler(search_service_type, search_location, min_rating, min_price, max_price);
    }
}

void MainClientView::showBookingMenu(Talent* talent, bool* stay_open) {
    
    static bool attatchment_menu = false;
    static const std::size_t BUFSIZE = 256;
    static char comments_buffer[BUFSIZE];
    static char date_buffer[BUFSIZE];
    static char file_buffer[BUFSIZE];
    ImGui::SetNextWindowSize(ImVec2(800, 1000), ImGuiCond_Always); 
    if (ImGui::Begin("Book Talent", NULL, ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Booking details for %s", talent->name.c_str());
        ImGui::Separator();

        ImGui::Text("Service Type: %s", talent->service_type.c_str());
        ImGui::Text("Location: %s", talent->location.c_str());
        ImGui::Text("Rating: %d/5", talent->rating);
        ImGui::Text("Price: $%.2f", talent->rate);

        ImGui::Separator();
        ImGui::InputText("Date", date_buffer, BUFSIZE);
        ImGui::InputTextMultiline("Comments", comments_buffer, BUFSIZE, ImVec2(200, 200));
        if(ImGui::Button("Add attatchment", ImVec2(300, 40))) {
            attatchment_menu = true;
            INFO("booking", "showing attatchment");
        }

        ImGui::Separator();
        if (ImGui::Button("Confirm Booking", ImVec2(150, 40))) {
            *stay_open = false;
            memset(comments_buffer, '\0', BUFSIZE);
            memset(date_buffer, '\0', BUFSIZE);
            memset(file_buffer, '\0', BUFSIZE);
            bookingHandler(talent);
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(150, 40))) {
            *stay_open = false;
            memset(comments_buffer, '\0', BUFSIZE);
            memset(date_buffer, '\0', BUFSIZE);
            memset(file_buffer, '\0', BUFSIZE);
        }

        if(attatchment_menu) {
            ImGui::InputText("File name", file_buffer, BUFSIZE);
            if(ImGui::Button("Upload", ImVec2(300, 40))) {
                memset(file_buffer, '\0', BUFSIZE);
                attatchment_menu = false;
                uploadHandler();
            }
            if(ImGui::Button("Cancel##", ImVec2(300, 40))) {
                memset(file_buffer, '\0', BUFSIZE);
                attatchment_menu = false;
            }
        }
        ImGui::End();
    }
}

void MainClientView::showMessageBox(bool* p_open, Talent* talent) {
    if(!*p_open) {
        return;
    }
    static char buffer[BUFFER_SIZE] = "\0";
    std::string name = "Message " + talent->name;
    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::InputTextMultiline("##bio", buffer, BUFFER_SIZE, ImVec2(400, 200));
    ImGui::SameLine();
    if(ImGui::SmallButton("Send")) {
        memset(buffer, '\0', BUFFER_SIZE);
        *p_open = false;
    }
    ImGui::SameLine();
    if(ImGui::SmallButton("Cancel")) {
        memset(buffer, '\0', BUFFER_SIZE);
        *p_open = false;
    }
}

void MainClientView::showTalentSearchResults() {
    static bool show_booking_menu = false;
    static bool show_message_box = false;
    static Talent* selected_talent = nullptr;

    if (search_results.empty()) {
        ImGui::Text("No talents found matching the criteria.");
        return;
    }

    // Display the talents in a table format
    if (ImGui::BeginTable("Talents Table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Name", 0, 150.0f);
        ImGui::TableSetupColumn("Service Type", 0, 200.0f);
        ImGui::TableSetupColumn("Location", 0, 150.0f);
        ImGui::TableSetupColumn("Rating", 0, 100.0f);
        ImGui::TableSetupColumn("Price", 0, 100.0f);
        ImGui::TableSetupColumn("Action", 0, 200.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < search_results.size(); ++i) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", search_results[i].name.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", search_results[i].service_type.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", search_results[i].location.c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%d/5", search_results[i].rating);
            ImGui::TableSetColumnIndex(4); ImGui::Text("$%.2f", search_results[i].rate);
            ImGui::TableSetColumnIndex(5);
            std::string book_button_label = "Book##" + std::to_string(i); 
            if (ImGui::Button(book_button_label.c_str())) {
                INFO("", "booking talent");
                show_booking_menu = true;
                selected_talent = &search_results[i];
            }
            std::string msg_button_label = "Message##" + std::to_string(i);
            ImGui::SameLine();
            if (ImGui::Button(msg_button_label.c_str())) {
                INFO("", "messaging talent");
                show_message_box = true;
                selected_talent = &search_results[i];
            }
        }
        ImGui::EndTable();
    }

    if (show_booking_menu && selected_talent != nullptr) {
        showBookingMenu(selected_talent, &show_booking_menu);
    }
    if(show_message_box && selected_talent != nullptr) {
        showMessageBox(&show_message_box, selected_talent);
    }
}

void MainClientView::render() {
    std::string welcome_msg =  "Welcome, " + user->getUsername() + ".";
	const char* title = welcome_msg.c_str();
	
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

	showMenuBar();

	ImGui::BeginChild("Talents List", ImVec2(0, 150), true);  
    showJobs();  
    ImGui::EndChild();

    ImGui::BeginChild("Talent Search Filters", ImVec2(0, 400), true);  
    showTalentSearchFilters();  
    ImGui::EndChild();

	ImGui::BeginChild("Talent Search Results", ImVec2(0, 0), true);  
    showTalentSearchResults();  
    ImGui::EndChild();

	ImGui::End();
}

