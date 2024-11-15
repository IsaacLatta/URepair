#include "View.h"
#include "App.h"

void MainAdminView::render() {
    static char query_input_buffer[BUFFER_SIZE] = "SELECT * FROM USERS";
    static char query_output_buffer[QUERY_BUFFER];
    
    if (ImGui::Button("LOGOUT", ImVec2(200, 50)))
    {
        logoutHandler();
    }

    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Input query: ", query_input_buffer, BUFFER_SIZE);
    
    if (ImGui::Button("Run query: ", ImVec2(200, 50))) {
        if (queryHandler) {
            LOG("INFO", "render", "query input buffer: %s", query_input_buffer);
            const char* query_output = queryHandler(query_input_buffer);
            
            LOG("INFO", "render", "Query result: %s", query_output);
            memset(query_output_buffer, '\0', QUERY_BUFFER);
    
            strncpy(query_output_buffer, query_output, QUERY_BUFFER - 1);
            query_output_buffer[QUERY_BUFFER - 1] = '\0';
        }
        else {
            ERROR("MainAdminView", "no query handler");
        }
    }
    //output query
    ImGui::Separator();
    ImGui::Text("Output Query:");
    ImGui::TextWrapped("%s", query_output_buffer);
}