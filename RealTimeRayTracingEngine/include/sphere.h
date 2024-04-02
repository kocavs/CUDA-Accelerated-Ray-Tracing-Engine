#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere: public hittable
{
private:
    point3 center;
    float radius;
    shared_ptr<material> mat_ptr;
public:
    sphere(){}
    sphere(point3 cen, float r, shared_ptr<material> mat): center(cen), radius(r), mat_ptr(mat){}
    
    point3 get_center() const {return center;}
    float get_radius() const {return radius;}
    shared_ptr<material> get_material() const {return mat_ptr;}

    void set_center(point3 cen){center = cen;}
    void set_radius(float r){radius = r;}
    void set_material(shared_ptr<material> mat){mat_ptr = mat;}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;



};
#endif
