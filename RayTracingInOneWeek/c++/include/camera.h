#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"
#include <cmath> // Make sure to include cmath for M_PI

// Convert degrees to radians
double degrees_to_radians(double degrees) {
    return degrees * (M_PI / 180.0);
}

class Camera {
public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    
    vec3 get_origin() const { return origin; }
    vec3 get_lower_left_corner() const { return lower_left_corner; }
    vec3 get_horizontal() const { return horizontal; }
    vec3 get_vertical() const { return vertical; }

    Camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = vec3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    }

    // Function to get ray from camera to viewport
    inline ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

    // Method to set or update the camera parameters
    inline void set_camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, double vfov, double aspect_ratio) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    }
};

#endif
