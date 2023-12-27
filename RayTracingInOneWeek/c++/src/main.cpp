#include <iostream>
#include <time.h>
#include <GLFW/glfw3.h>

//Dear imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "vec3.h"
#include "ray.h"
#include "camera.h"

using std::cout;

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(vec3 light_position, color light_color, float light_intensity, const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 hit_point = r.at(t);
        vec3 normal = unit_vector(hit_point - vec3(0, 0, -1)); // Normal at the hit point

        vec3 to_light = unit_vector(light_position - hit_point); // Direction from hit point to light
        color effective_light_color = light_intensity * light_color;

        // Simple diffuse lighting calculation
        auto diffuse = std::max(dot(normal, to_light), 0.0f);
        color diffuse_color = diffuse * effective_light_color; // Use effective_light_color here

        // Combine with the object's base color
        color object_color = color(1, 1, 1); // Base color of the object
        return diffuse_color * object_color;
    }

    // Background color
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main(){

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
    Camera cam;
    vec3 camera_position = cam.get_origin(); // Camera position
    vec3 camera_target = vec3(0, 0, -1); // Where the camera is looking

    //image setup
    const auto aspect_ratio = 16.0 / 9.0;
    const int img_width = 800;
    const int img_height = static_cast<int>(img_width / aspect_ratio);

    //ligth setup
    vec3 light_position = vec3(-1, -1, -1); // Initial light position
    color light_color = color(1, 1, 1); // Initial light color
    float light_intensity = 1.0; // Initial light intensity

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

        ImGui::Begin("Light Controls");
        ImGui::SliderFloat3("Light Position", &light_position[0], -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Color", &light_color[0]);
        ImGui::SliderFloat("Light Intensity", &light_intensity, 0.0f, 10.0f);
        ImGui::End();

        // Update the camera based on the ImGui input
        cam.set_camera(camera_position, camera_target - camera_position, vec3(0, 1, 0), 60.0, aspect_ratio);
        
        for (int j = img_height-1; j >= 0; --j) {
            for (int i = 0; i < img_width; ++i) {
                //normalize the view point 
                auto u = double(i) / (img_width-1);
                auto v = double(j) / (img_height-1);

                //construct the ray from camera
                ray r(cam.get_origin(), cam.get_lower_left_corner() + u*cam.get_horizontal() + v*cam.get_vertical() - cam.get_origin());

                color pix_col = ray_color(light_position, light_color, light_intensity, r);

                int index = (j * img_width + i) * 3;
                pixels[index] = static_cast<unsigned char>(pix_col.x() * 255.99);
                pixels[index + 1] = static_cast<unsigned char>(pix_col.y() * 255.99);
                pixels[index + 2] = static_cast<unsigned char>(pix_col.z() * 255.99);
            }
        }

        // Update the texture with the ray tracing result
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_width, img_height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        // Render the texture using ImGui
        ImGui::Begin("Ray Traced Image");
        ImGui::Image((void*)(intptr_t)texture, ImVec2(img_width, img_height));
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