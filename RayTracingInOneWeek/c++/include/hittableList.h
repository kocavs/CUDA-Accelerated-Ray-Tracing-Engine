#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittableList : public hittable
{
private:
    //a list of hittable pointers
    std::vector<shared_ptr<hittable>> objects;

public:
    hittableList() {}
    hittableList(shared_ptr<hittable> obj){add(obj);}

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    virtual bool hit( const ray& r, float t_min, float t_max, hit_record& rec) const override;
};

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
#endif
