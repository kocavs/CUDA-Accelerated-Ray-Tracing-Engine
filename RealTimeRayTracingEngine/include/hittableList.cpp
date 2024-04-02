#include "hittableList.h"

bool hittableList::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    hit_record tmp;
    bool is_hit = false;
    float cloest_p = t_max;

    for (const auto &object : objects)
    {
        if(object->hit(r, t_min, cloest_p, tmp)){
            is_hit = true;
            cloest_p = tmp.t;
            rec = tmp;
        }
    }
    return is_hit;
}