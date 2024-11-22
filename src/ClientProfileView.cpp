#include "View.h"
#include "App.h"

void ClientProfileView::render () {
    ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin("Profile", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
    showOptions();

    ImGui::BeginChild("Client Profile");
    showProfileInfo();
    // render additional info here
    ImGui::EndChild();

    ImGui::End();
}
