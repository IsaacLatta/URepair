#include "View.h"
#include "App.h"

void MainAdminView::render() {
    static char query_input_buffer[BUFFER_SIZE];
    static char query_output_buffer[BUFFER_SIZE];
    
    if (ImGui::Button("LOGOUT", ImVec2(200, 50)))
    {
        logoutHandler();
    }

    ImGui::SetNextItemWidth(300.0f);  // Set width of input field
    ImGui::InputText("Input query: ", query_input_buffer, BUFFER_SIZE);
    
    if (ImGui::Button("Run query: ", ImVec2(200, 50))) {
        if (queryHandler) {
            const char* query_output = queryHandler(query_input_buffer);
            //clear output buffer
            memset(query_output_buffer, '\0', BUFFER_SIZE);
            //copy query output to output buffer
            strncpy(query_output_buffer, query_output, BUFFER_SIZE - 1);
            query_output_buffer[BUFFER_SIZE - 1] = '\0';
        }
        else {
            ERROR("MainAdminView", "no query handler");
        }
    }
    //output query
    ImGui::Separator();
    ImGui::Text("Output Query:");
    ImGui::TextWrapped("%s", query_output_buffer);
    //reset query buffer (optional)
    memset(query_input_buffer, '\0', BUFFER_SIZE);
    

}