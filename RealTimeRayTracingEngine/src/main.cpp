#include <iostream>
#include <time.h>
#include <GLFW/glfw3.h>
#include <chrono>

//Dear imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "renderer.h"
#include "sphere.h"
#include "material.h"

hittableList random_scene() {
    hittableList world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -2; a < 2; a++) {
        for (int b = -2; b < 2; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main(){
    std::chrono::time_point<std::chrono::steady_clock> lastFrameTime = std::chrono::steady_clock::now();
    double deltaTime = 0.0;

    const auto aspect_ratio = 16.0 / 9.0;

    //GUI setup
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracing", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //image setup
    const int img_width = 800;
    const int img_height = static_cast<int>(img_width / aspect_ratio);
    const int samples_per_pixel = 1;
    const int max_depth = 20;

    Renderer renderer(img_width, img_height, samples_per_pixel, max_depth);

    //camera setup
    Camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);;
    vec3 camera_position = cam.get_origin(); // Camera position
    vec3 camera_target = vec3(0, 0, 0); // Where the camera is looking
    vec3 camera_direction = unit_vector(camera_target - camera_position);

    //world setup
    hittableList world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    renderer.setScene(world, cam);
    
    //Dear imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Allocate memory for the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    while (!glfwWindowShouldClose(window)) {
        auto currentFrameTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<double>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        double fps = 1.0 / deltaTime;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Display FPS using ImGui
        ImGui::Begin("Performance");
        ImGui::Text("FPS: %.1f", fps);
        ImGui::End();

        
        ImGui::Begin("Object Controls");
        std::shared_ptr<material> mat_lam;
        for(int i = 0; i < world.length(); i++){
            auto obj = world.get(i);
            auto sphere_ptr = std::dynamic_pointer_cast<sphere>(obj);

            if (sphere_ptr) {
                vec3 pos = sphere_ptr->get_center();
                ImGui::SliderFloat3(("Position##" + std::to_string(i)).c_str(), &(pos[0]), -10.0f, 10.0f);
                sphere_ptr->set_center(pos);

                auto mat = sphere_ptr->get_material();
                if (auto lam = std::dynamic_pointer_cast<lambertian>(mat)) {
                    color col = lam->get_albedo();
                    ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), &col[0]);
                    lam->set_albedo(col);
                } else if (auto met = std::dynamic_pointer_cast<metal>(mat)) {
                    color col = met->get_albedo();
                    ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), &col[0]);
                    met->set_albedo(col);
                }
            }
        }
        ImGui::End();
        
        const float cameraSpeed = 0.05f; // adjust as needed
        camera_direction = unit_vector(camera_target - camera_position);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera_position += cameraSpeed * camera_direction;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera_position -= cameraSpeed * camera_direction;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera_position -= unit_vector(cross(camera_direction, vec3(0, 1, 0))) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_position += unit_vector(cross(camera_direction, vec3(0, 1, 0))) * cameraSpeed;

        // Update the camera based on the User input
        cam.set_camera(camera_position, camera_target, vec3(0, 1, 0), 90.0f, aspect_ratio);
        renderer.updateCamera(cam);

        renderer.renderScene();

        // Update the texture with the ray tracing result
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_width, img_height, GL_RGB, GL_UNSIGNED_BYTE, renderer.getPixels().data());

        // Render the texture using ImGui
        ImGui::Begin("Ray Traced Image");
        ImGui::Image((void*)(intptr_t)texture, ImVec2(img_width, img_height), ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    glDeleteTextures(1, &texture);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
