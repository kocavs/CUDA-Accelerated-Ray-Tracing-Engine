#ifndef CAMERA_H
#define CAMERA_H

#include "hitUtils.h"
#include <cmath>

class Camera {
private:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

public:
    vec3 get_origin() const { return origin; }
    vec3 get_lower_left_corner() const { return lower_left_corner; }
    vec3 get_horizontal() const { return horizontal; }
    vec3 get_vertical() const { return vertical; }

    // vertical field-of-view in degrees
    Camera(point3 lookfrom, point3 lookat, vec3 vup, float vfov, float aspect_ratio) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0f * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    }

    // Function to get ray from camera to viewport
    ray get_ray(float s, float t) const {
        return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    }

    // Method to set or update the camera parameters
    inline void set_camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, float vfov, float aspect_ratio) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0f * h;
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
