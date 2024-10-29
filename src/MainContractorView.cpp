#include "View.h"
#include "App.h"

void MainContractorView::showContractorMenuBar() {
	bool change_to_profile = false;
	bool logout = false;
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
			ImGui::MenuItem("Jobs");
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
}
void MainContractorView::showJobRequests() {

}
void MainContractorView::handle() {

}