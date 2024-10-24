#include "app.h"

// Framebuffer size callback for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

App::App() : main_window(nullptr) {}

bool App::init() {
    if (!glfwInit()) {
        ERROR("GLFW", "init failed");
        return false;
    }

    // Set GLFW options and create a window with an OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Main Window", NULL, NULL);
    if (!window) {
        ERROR("GLFW", "window creation failed");
        glfwTerminate();
        return false;  // Return false here
    }

    this->main_window = window;
    glfwMakeContextCurrent(this->main_window);
    glfwSetFramebufferSizeCallback(this->main_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        ERROR("GLAD", "init of OpenGL loader failed");
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Initialize ImGui for GLFW and OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(this->main_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");  // Use the appropriate OpenGL version
    
    // OpenGL settings
    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    return true;
}


void App::mainLoop() {
	while(!glfwWindowShouldClose(this->main_window)) {
		glfwPollEvents();
		// Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		ImGui::Begin("Hello!");
		ImGui::Text("This is a window.");
		if(ImGui::Button("Click Me")) {
			INFO("Button", "clicked");
		}
		ImGui::End();

		ImGui::Render();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Swap buffers
		glfwSwapBuffers(this->main_window);
        
	}
}

void App::run() {
	ImGui::StyleColorsDark();
	mainLoop();	
	
	// Cleanup ImGui and GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
