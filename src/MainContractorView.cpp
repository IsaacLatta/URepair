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
		if (ImGui::BeginMenu("Profile"))
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

void MainContractorView::showJobRequestInfo(const Job* job, bool* stay_open) {
	ImGui::Text("Job Description");
	static bool attatchment_menu = false;
	static const std::size_t BUFSIZE = 256;
}

void MainContractorView::showJobRequests() {
	ImGui::Text("Pending job requests: %s");
	static bool show_request_info = false;
	std::vector<Job>* jobs = user->getJobs();
	for (const Job& job : *jobs){
		if (ImGui::Selectable(job.name)) {
			if (ImGui::Button("View"))
			{
				showJobRequestInfo(&job, &show_request_info);
			}
			
		}

	}
}
void MainContractorView::handle() {
	std::string welcome_screen = "Welcome, " + user->getUsername() + ".";
	const char* title = welcome_screen.c_str();

	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

	showContractorMenuBar();
	showJobRequests();
	ImGui::End();
}