#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray{
    public:
        vec3 ori;
        vec3 dir;

        ray(){};
        ray(vec3 origin, vec3 dir): ori(origin), dir(dir) {};

        vec3 origin() const {return ori; }
        vec3 direction() const {return dir;}

        vec3 at(float t) const {
            return ori + t*dir;
        }
};
#endif