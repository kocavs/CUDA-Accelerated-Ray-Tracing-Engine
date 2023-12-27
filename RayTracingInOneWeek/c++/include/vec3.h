#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <iostream>

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

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.val[0], t*v.val[1], t*v.val[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
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
#endif

