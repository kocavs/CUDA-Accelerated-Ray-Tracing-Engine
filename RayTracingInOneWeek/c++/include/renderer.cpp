#include "renderer.h"
#include "material.h"

Renderer::Renderer(int width, int height, int samplesPerPixel, int maxDepth)
    : imgWidth(width), imgHeight(height), spp(samplesPerPixel), maxDepth(maxDepth) {
    pixels.resize(width * height * 3);
}

void Renderer::setScene(const hittableList& world, const Camera& camera) {
    this->world = world;
    this->camera = camera;
}

void Renderer::renderScene() {
    for (int j = 0; j < imgHeight; j++) {
        for (int i = 0; i < imgWidth; i++) {
            color pix_col;
            //clamping with multi-sampled pixels
            for (int s = 0; s < spp; s++){
                //normalize the view point 
                auto u = (i + random_float()) / (imgWidth-1);
                auto v = (j + random_float()) / (imgHeight-1);

                //construct the ray from camera
                //color pix_col = ray_color(light_position, light_color, light_intensity, r, world);
                ray rLight = camera.get_ray(u, v);
                pix_col += ray_color(rLight, world, maxDepth);
            }
            
            auto r = pix_col.x();
            auto g = pix_col.y();
            auto b = pix_col.z();

            auto scale = 1.0 / spp;
            r = sqrt(scale * r);
            g = sqrt(scale * g);
            b = sqrt(scale * b);

            int index = (j * imgWidth + i) * 3;
            pixels[index] = static_cast<unsigned char>(clamp(r, 0.0, 0.999) * 256);
            pixels[index + 1] = static_cast<unsigned char>(clamp(g, 0.0, 0.999) * 256);
            pixels[index + 2] = static_cast<unsigned char>(clamp(b, 0.0, 0.999) * 256);
        }
    }
}

const std::vector<unsigned char>& Renderer::getPixels() const {
    return pixels;
}

color Renderer::ray_color(const ray& r, hittableList world, int depth){
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

void Renderer::updateCamera(const Camera &cam){
    this->camera = cam;
}