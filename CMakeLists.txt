cmake_minimum_required(VERSION 3.10)
project(URepair)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Set the OpenGL preference to GLVND (modern OpenGL driver)
set(OpenGL_GL_PREFERENCE GLVND)

# Define source files
set(SOURCE_FILES
    src/main.cpp
    src/App.cpp
    src/logger.cpp
    src/User.cpp
    src/MainClientView.cpp
    src/LoginView.cpp
    src/Database.cpp
    src/ProfileView.cpp
    src/Controller.cpp
    src/MainContractorView.cpp
    src/MainAdminView.cpp
    src/SQLite.cpp
    third_party/glad/glad.c
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
    third_party/sqlite/sqlite3.c
)

# Add the project include directory
include_directories(include)

# Add glad include directory
include_directories(third_party/glad/include)

# Add imgui include directory
include_directories(third_party/imgui)
include_directories(third_party/imgui/backends)

# Add GLFW include directory (common to both platforms)
include_directories(third_party/glfw)

#Add Sqlite directories
include_directories(third_party/sqlite)

# Find the OpenGL package (for cross-platform OpenGL linking)
find_package(OpenGL REQUIRED)

# Platform-specific settings
if (WIN32)
    # On Windows, use the local .lib files for linking
    link_directories(third_party/glfw/lib/windows)
    
    # Add the executable
    add_executable(URepair ${SOURCE_FILES})
    
    # Link with the glfw3.lib and OpenGL32.lib
    target_link_libraries(URepair glfw3 OpenGL::GL)
    
    # Ensure glfw3.dll is copied to the output folder
    add_custom_command(TARGET URepair POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/third_party/glfw/bin/windows/glfw3.dll
        $<TARGET_FILE_DIR:URepair>)

elseif(UNIX)
    # On Linux, use the .so files for linking
    link_directories(third_party/glfw/bin/linux)
    
    # Add the executable
    add_executable(URepair ${SOURCE_FILES})
    
    # Link with the GLFW and OpenGL libraries
    target_link_libraries(URepair glfw OpenGL::GL dl)
endif()
