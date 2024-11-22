#include "View.h"
#include "App.h"

void MainContractorView::showContractorMenuBar() {
	bool change_to_profile = false;
	bool logout = false;
	bool change_to_jobs = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Profile")) {
			if (ImGui::MenuItem("Logout")) {
				logout = true;
			}
			ImGui::EndMenu();
			
		}
		if (ImGui::BeginMenu("Profile")) {
			if (ImGui::MenuItem("View Profile")) {
				change_to_profile = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Profile")){
			if (ImGui::MenuItem("Jobs")) {
				change_to_jobs = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (change_to_profile) {
		profileHandler();
	}
	if (logout) {
		logoutHandler();
	}
	if (change_to_jobs) {
		INFO("view", "Active job handler would appear");
	}
}
void MainContractorView::showJobRequestInfo(Job* job, bool* stay_open) {
	if(!*stay_open) {
		return;
	}
	ImGui::SetNextWindowSize(ImVec2(800, 1000), ImGuiCond_Always);
	if (ImGui::Begin("Request Info:", stay_open, ImGuiWindowFlags_NoResize)) {
		ImGui::Text("Job Name: %s", job->name);
		
		//accept project 
		if (ImGui::Button("Accept")) {
			INFO("Contractor", "Job accepted");
			jobAcceptHandler(job);
			*stay_open = false;
		}
		//deny project
		if (ImGui::Button("Deny")) {
			INFO("Contractor", "Job denied");
			jobAcceptHandler(job);
			*stay_open = false;
		}
	}
	ImGui::End();
}
void MainContractorView::showActiveJobs() {
	ImGui::Text("Active Jobs: ");


}

void MainContractorView::showJobRequests() {
	static bool show_request_menu = false;
	std::vector<Job>* jobs = user->getJobs();
	static Job* selected_job = nullptr; /* Job pointer needs to be static, othewise it gets reset to nullptr every frame */
												/* Old: const Job* = ... | New: static const Job* = ... */
	ImGui::Text("Pending job requests: %d", static_cast<int>(jobs->size()));
	for (int i = 0; i < jobs->size(); i++) {
		ImGui::PushID(i);
		Job& job = jobs->at(i);
		ImGui::Text("Job Request from: %s", job.name);
		ImGui::SameLine();
			if (ImGui::Button("View##Button"))
			{
				show_request_menu = true;
				selected_job = &job;
			}
			ImGui::PopID();
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
	
	showJobRequests();
	showActiveJobs();
	ImGui::End();
}