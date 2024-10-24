#include "AppState.h"
#include "app.h"

void MainState::showMenu(bool* p_open) {
	ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	ImGui::Begin("Menu", p_open, ImGuiWindowFlags_MenuBar);

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Say yes")) {
			if(ImGui::MenuItem("Are you sure?")) {
				INFO("menu", "yes");
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Say No")) {
			if(ImGui::BeginMenu("Are you sure")) {
				if(ImGui::MenuItem("Yes")) {
					INFO("menu", "no");
				}
				if(ImGui::MenuItem("No")) {
					INFO("menu", "good call");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void MainState::showMainWindow() {
	ImGui::Begin("Hello!");
	ImGui::Text("This is a window.");
	if(ImGui::Button("Click Me")) {
		INFO("Button", "clicked");
	}
	ImGui::End();

	bool show_menu = true;
	showMenu(&show_menu);
}

void MainState::handle() {
    showMainWindow();
}

