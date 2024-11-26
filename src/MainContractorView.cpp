#include "View.h"
#include "App.h"

void MainContractorView::showAllJobs(bool* stay_open) {
	if(!*stay_open) {
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin("ALL JOBS", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

	if(ImGui::Button("Close")) {
		*stay_open =false;
	}

	std::vector<Job>* jobs = user->getJobs();
	
	if (ImGui::BeginTable("Jobs", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Name", 0, 100.0f);
        ImGui::TableSetupColumn("Status", 0, 100.0f);
        ImGui::TableSetupColumn("Date", 0, 100.0f);
        ImGui::TableSetupColumn("Pay out", 0, 100.0f);
        ImGui::TableSetupColumn("Description", 0, 200.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < jobs->size(); ++i) {
			auto job = jobs->at(i);
			
			ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", job.name);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", job.status);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", job.date);
            ImGui::TableSetColumnIndex(3); ImGui::Text("$%.2f", job.cost);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", job.description);
        }
        ImGui::EndTable();
    }
	ImGui::End();
}

void MainContractorView::showContractorMenuBar() {
	static bool change_to_profile = false;
	static bool logout = false;
	static bool show_all_jobs = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Profile")) {
			if (ImGui::MenuItem("Logout")) {
				logout = true;
			}
			if (ImGui::MenuItem("View Profile")) {
				change_to_profile = true;
			}
			if (ImGui::MenuItem("Jobs")) {
				show_all_jobs = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (change_to_profile) {
		change_to_profile = false;
		profileHandler();
	}
	if (logout) {
		logoutHandler();
	}
	if (show_all_jobs) {
		showAllJobs(&show_all_jobs);
	}
}
void MainContractorView::showJobRequestInfo(Job* job, bool* stay_open) {
	if(!*stay_open) {
		return;
	}
	ImGui::SetNextWindowSize(ImVec2(800, 1000), ImGuiCond_Always);
	if (ImGui::Begin("Request Info:", stay_open, ImGuiWindowFlags_NoResize)) {
		ImGui::Text("Job Name: %s", job->name);
		
		if (ImGui::Button("Accept")) {
			INFO("Contractor", "Job accepted");
			jobAcceptHandler(job, true);
			*stay_open = false;
		}

		if (ImGui::Button("Deny")) {
			INFO("Contractor", "Job denied");
			jobAcceptHandler(job, true);
			*stay_open = false;
		}
	}
	ImGui::End();
}
void MainContractorView::showActiveJobs() {
	std::vector<Job>* jobs = user->getJobs();
	
	if (ImGui::BeginTable("Pending Jobs", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Name", 0, 100.0f);
        ImGui::TableSetupColumn("Status", 0, 100.0f);
        ImGui::TableSetupColumn("Date", 0, 100.0f);
        ImGui::TableSetupColumn("Pay out", 0, 100.0f);
        ImGui::TableSetupColumn("Description", 0, 200.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < jobs->size(); ++i) {
			auto job = jobs->at(i);
			if(strcmp(job.status, "in progress")) { // Job not in progress
				continue;
			}
			
			ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", job.name);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", job.status);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", job.date);
            ImGui::TableSetColumnIndex(3); ImGui::Text("$%.2f", job.cost);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", job.description);
        }
        ImGui::EndTable();
    }
	
}

void MainContractorView::showJobRequests() {
	static bool show_request_menu = false;
	std::vector<Job>* jobs = user->getJobs();
	static Job* selected_job = nullptr; 
												
	if (ImGui::BeginTable("Pending Jobs", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Name", 0, 100.0f);
        ImGui::TableSetupColumn("Status", 0, 100.0f);
        ImGui::TableSetupColumn("Date", 0, 100.0f);
        ImGui::TableSetupColumn("Pay out", 0, 100.0f);
        ImGui::TableSetupColumn("Description", 0, 200.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < jobs->size(); ++i) {
			auto job = jobs->at(i);
			if(strcmp(job.status, "pending")) { // Job not pending
				continue;
			}
			
			ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", job.name);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", job.status);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", job.date);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%d/5", job.cost);
            ImGui::TableSetColumnIndex(4); ImGui::Text("$%.2f", job.description);
            std::string label = "View##" + std::to_string(i); 
			if (ImGui::Button(label.c_str())) {
                INFO("", "selected job");
                show_request_menu = true;
				selected_job = &job;
            }
        }
        ImGui::EndTable();
    }
	if (show_request_menu && selected_job != nullptr) {
		showJobRequestInfo(selected_job, &show_request_menu);
	}
}

void MainContractorView::render() {
	std::string welcome_screen = "Welcome, " + user->getUsername() + ".";
	const char* title = welcome_screen.c_str();

	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	showContractorMenuBar();

	ImGui::BeginChild("Active Jobs", ImVec2(0, 100), true);  
	showActiveJobs();
	ImGui::EndChild();
	
	ImGui::BeginChild("Pending Jobs", ImVec2(0, 400), true);  
	showJobRequests();
	ImGui::EndChild();

	ImGui::End();
}