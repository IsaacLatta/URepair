cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set the OpenGL preference to GLVND (modern)
set(OpenGL_GL_PREFERENCE GLVND)

# Define source files
set(SOURCE_FILES
    src/main.cpp
    third_party/glad/glad.c
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Add glad include directory
include_directories(third_party/glad/include)

# Add imgui include directory
include_directories(third_party/imgui)
include_directories(third_party/imgui/backends)

# Find GLFW (cross-platform)
find_package(glfw3 3.3 REQUIRED)

# Find OpenGL (cross-platform)
find_package(OpenGL REQUIRED)

# Create the executable
add_executable(myapp ${SOURCE_FILES})

# Link libraries (cross-platform linking)
target_link_libraries(myapp glfw OpenGL::GL)
