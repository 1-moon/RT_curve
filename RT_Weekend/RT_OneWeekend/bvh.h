#ifndef BVH_H
#define BVH_H


#include <algorithm>
#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"



class bvh_node : public Hittable {
public:
    bvh_node(const Hittable_list& list) : bvh_node(list.objs, 0, list.objs.size()) {}

    bvh_node(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        }
        else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }

        bbox = Aabb(left->BoundingBox(), right->BoundingBox());
    }

    bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->TestIntersection(r, ray_t, rec);
        bool hit_right = right->TestIntersection(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    Aabb BoundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    Aabb bbox;

    static bool box_compare(
        const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index
    ) {
        return a->BoundingBox().axis(axis_index).min < b->BoundingBox().axis(axis_index).min;
    }

    static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 2);
    }
};


#endif