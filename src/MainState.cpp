#include "AppState.h"
#include "app.h"

void MainState::showMenuBar() {
	bool change_to_profile = false;
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Logout");  // Logout menu item
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Profile")) {
            if(ImGui::MenuItem("View Profile")) {
				change_to_profile = true;
			}  
            ImGui::MenuItem("Jobs");  
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
	if(change_to_profile) {
		//app->changeState(new ProfileState(app));
		INFO("State", "would change to profile");
	}
}

void MainState::showJobs() {
	std::vector<Job>* jobs = app->getUser()->getJobs();
	
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

void MainState::handle() {
    std::string welcome_msg =  "Welcome, " + app->getUser()->getUsername() + ".";
	const char* title = welcome_msg.c_str();
	
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

	showMenuBar();

	ImGui::BeginChild("Talents List", ImVec2(0, 0), true);  
    showJobs();  
    ImGui::EndChild();

	ImGui::End();
}

