cmake_minimum_required(VERSION 3.10)
project(RayTracing)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(include)
include_directories(libs/imgui)
include_directories(libs/imgui/backends)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_widgets.cpp libs/imgui/imgui_tables.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
