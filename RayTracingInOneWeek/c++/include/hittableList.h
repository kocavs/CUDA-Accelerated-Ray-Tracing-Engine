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
    int length(){return objects.size();}
    shared_ptr<hittable> get(int index) const {
        if (index < 0 || index >= objects.size()) {
            return nullptr; // or throw an exception
        }
        return objects[index];
    }
    virtual bool hit( const ray& r, float t_min, float t_max, hit_record& rec) const override;
};
#endif
