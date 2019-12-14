#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "common/rtweekend.h"
#include "hittable.h"
#include <vector>


class hittable_list: public hittable  {
    public:
        hittable_list() {}

        void clear();
        void add(hittable *object);

        virtual bool bounding_box(double t0, double t1, aabb& output_box) const;
        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
        virtual double pdf_value(const vec3 &o, const vec3 &v) const;
        virtual vec3 random(const vec3 &o) const;

        std::vector<hittable*> objects;
};


void hittable_list::clear() {
    objects.clear();
}


void hittable_list::add(hittable *object) {
    objects.push_back(object);
}


bool hittable_list::bounding_box(double t0, double t1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_true = objects[0]->bounding_box(t0, t1, temp_box);

    if (!first_true)
        return false;
    else
        output_box = temp_box;

    for (hittable* object : objects) {
        if (object->bounding_box(t0, t1, temp_box))
            output_box = surrounding_box(output_box, temp_box);
        else
            return false;
    }

    return true;
}


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (hittable *object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}


double hittable_list::pdf_value(const vec3 &o, const vec3 &v) const {
    auto weight = 1.0/objects.size();
    auto sum = 0.0;

    for (auto object : objects)
        sum += weight * object->pdf_value(o, v);

    return sum;
}


vec3 hittable_list::random(const vec3 &o) const {
    auto int_size = static_cast<int>(objects.size());
    auto index = static_cast<size_t>(random_int(0, int_size-1));
    return objects[index]->random(o);
}


#endif
