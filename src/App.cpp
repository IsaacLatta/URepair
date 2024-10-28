#include "App.h"

// Framebuffer size callback for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

App::App() : main_window(nullptr), controller(nullptr) {}

App::~App() {
	if(controller != nullptr) delete controller;
}

bool App::init() {
    if (!glfwInit()) {
        ERROR("GLFW", "init failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);  // Alpha channel for transparency
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);  // Transparent window (if supported)
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Main Window", NULL, NULL);
    if (!window) {
        ERROR("GLFW window", get_gl_error().c_str());
        glfwTerminate();
        return false;  
    }

    this->main_window = window;
    glfwMakeContextCurrent(this->main_window);
	glfwSwapInterval(1); // Adjust to refresh rate
    glfwSetFramebufferSizeCallback(this->main_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        ERROR("GLAD", "init of OpenGL loader failed");
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(this->main_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");  // Use the appropriate OpenGL version
    
    // OpenGL settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    ImGui::GetIO().FontGlobalScale = 1.25f;  // Scale the entire UI by 1.25x

    controller = new Controller();
	return controller->start();
}

void App::mainLoop() {
	while(!glfwWindowShouldClose(this->main_window)) {
		glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        controller->renderCurrent();

		ImGui::Render();
		glClearColor(0.0f, 0.125f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(this->main_window);
	}
	INFO("App", "window closed");
}

void App::run() {
	mainLoop();	
	
	INFO("App", "cleaning up ...");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
	INFO("App", "cleanup successful");
}
