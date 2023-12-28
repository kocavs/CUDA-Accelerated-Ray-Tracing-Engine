#include <iostream>
#include <time.h>
#include <GLFW/glfw3.h>

//Dear imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "hitUtils.h"

#include "material.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"

color ray_color(/*vec3 light_position, color light_color, float light_intensity, */const ray& r, hittableList& world, int depth) {
    hit_record hit;

    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, hit)) {
        // vec3 hit_point = r.at(hit.t);
        // vec3 normal = unit_vector(hit_point - vec3(0, 0, -1)); // Normal at the hit point

        // vec3 to_light = unit_vector(light_position - hit_point); // Direction from hit point to light
        // color effective_light_color = light_intensity * light_color;

        // // Simple diffuse lighting calculation
        // auto diffuse = std::max(dot(normal, to_light), 0.0f);
        // color diffuse_color = diffuse * effective_light_color; // Use effective_light_color here

        // // Combine with the object's base color
        // color object_color = hit.normal + color(1, 1, 1); // Base color of the object
        // return diffuse_color * object_color;
        ray scattered;
        color attenuation;
        if (hit.mat_ptr->scatter(r, hit, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    // Background color
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5f *(unit_direction.y() + 1.0f);
    return (1.0f-t)*color(1.0f, 1.0f, 1.0f) + t*color(0.5f, 0.7f, 1.0f);
}


int main(){
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
    // std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    //camera setup
    Camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);;
    vec3 camera_position = cam.get_origin(); // Camera position
    vec3 camera_target = vec3(0, 0, 0); // Where the camera is looking

    //image setup
   
    const int img_width = 800;
    const int img_height = static_cast<int>(img_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 20;

    // //ligth setup
    // vec3 light_position = vec3(-1, -1, -1); // Initial light position
    // color light_color = color(1, 1, 1); // Initial light color
    // float light_intensity = 1.0; // Initial light intensity

    //world setup
    hittableList world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

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

    std::vector<unsigned char> pixels(img_width * img_height * 3);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Camera Controls");
        ImGui::SliderFloat3("Camera Position", &camera_position[0], -10.0f, 10.0f);
        ImGui::SliderFloat3("Camera Target", &camera_target[0], -10.0f, 10.0f);
        ImGui::End();

        // ImGui::Begin("Light Controls");
        // ImGui::SliderFloat3("Light Position", &light_position[0], -10.0f, 10.0f);
        // ImGui::ColorEdit3("Light Color", &light_color[0]);
        // ImGui::SliderFloat("Light Intensity", &light_intensity, 0.0f, 10.0f);
        // ImGui::End();

        // Update the camera based on the ImGui input
        cam.set_camera(camera_position, camera_target - camera_position, vec3(0, 1, 0), 90.0f, aspect_ratio);
        
        for (int j = 0; j < img_height; j++) {
            for (int i = 0; i < img_width; i++) {
                color pix_col;
                //clamping with multi-sampled pixels
                for (int s = 0; s < samples_per_pixel; s++){
                    //normalize the view point 
                    auto u = (i + random_float()) / (img_width-1);
                    auto v = (j + random_float()) / (img_height-1);

                    //construct the ray from camera
                    //color pix_col = ray_color(light_position, light_color, light_intensity, r, world);
                    ray rLight = cam.get_ray(u, v);
                    pix_col += ray_color(rLight, world, max_depth);
                }
                
                auto r = pix_col.x();
                auto g = pix_col.y();
                auto b = pix_col.z();

                auto scale = 1.0 / samples_per_pixel;
                r = sqrt(scale * r);
                g = sqrt(scale * g);
                b = sqrt(scale * b);

                int index = (j * img_width + i) * 3;
                pixels[index] = static_cast<unsigned char>(clamp(r, 0.0, 0.999) * 256);
                pixels[index + 1] = static_cast<unsigned char>(clamp(g, 0.0, 0.999) * 256);
                pixels[index + 2] = static_cast<unsigned char>(clamp(b, 0.0, 0.999) * 256);
            }
        }

        // Update the texture with the ray tracing result
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_width, img_height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

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
