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

#include "hittable.h"
#include <vector>


class hittable_list: public hittable  {
    public:
        hittable_list() {}

        void clear();
        void add(hittable *object);
        size_t size();

        virtual bool hit(const ray &r, double tmin, double tmax, hit_record &rec) const;

        std::vector<hittable*> objects;
};

void hittable_list::clear() {
    objects.clear();
}

size_t hittable_list::size() {
    return objects.size();
}

void hittable_list::add(hittable *object) {
    objects.push_back(object);
}

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
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

#endif
