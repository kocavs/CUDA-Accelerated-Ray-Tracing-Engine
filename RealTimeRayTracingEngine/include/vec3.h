#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <iostream>
#include "hitUtils.h"

using std::sqrt;
class vec3{
    public:
        float val[3];

        // Default constructor
        vec3() : val{0, 0, 0}{}
        vec3(float x, float y, float z): val{x, y, z}{}

        float x() const { return val[0]; }
        float y() const { return val[1]; }
        float z() const { return val[2]; }

        vec3 operator-() const {return vec3{-val[0], -val[1], -val[2]};}

        //return the value of the i element
        float operator[](int i) const {return val[i];}

        //return the reference of the i element
        float& operator[](int i) {return val[i];}

        vec3& operator+= (const vec3& v){
            val[0] += v.val[0];
            val[1] += v.val[1];
            val[2] += v.val[2];
            return *this;
        }

        vec3& operator-= (const vec3& v){
            val[0] -= v.val[0];
            val[1] -= v.val[1];
            val[2] -= v.val[2];
            return *this;
        }
        
        vec3& operator*= (const float t){
            val[0] *= t;
            val[1] *= t;
            val[2] *= t;
            return *this;
        }

        vec3& operator/=(const float t) {
            return *this *= 1/t;
        }

        float length_squared() const {
            return val[0]*val[0] + val[1]*val[1] + val[2]*val[2];
        }

        float length() const {
            return sqrt(length_squared());
        }

        inline static vec3 random() {
            return vec3(random_float(), random_float(), random_float());
        }

        inline static vec3 random(float min, float max) {
            return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
        }

         bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(val[0]) < s) && (fabs(val[1]) < s) && (fabs(val[2]) < s);
        }
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions
// Overload the stream insertion operator
inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.val[0] << ", " << v.val[1] << ", " << v.val[2] << ")";
    return os;
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.val[0] + v.val[0], u.val[1] + v.val[1], u.val[2] + v.val[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.val[0] - v.val[0], u.val[1] - v.val[1], u.val[2] - v.val[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.val[0] * v.val[0], u.val[1] * v.val[1], u.val[2] * v.val[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.val[0], t*v.val[1], t*v.val[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u.val[0] * v.val[0]
        + u.val[1] * v.val[1]
        + u.val[2] * v.val[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.val[1] * v.val[2] - u.val[2] * v.val[1],
                u.val[2] * v.val[0] - u.val[0] * v.val[2],
                u.val[0] * v.val[1] - u.val[1] * v.val[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    } 
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif

