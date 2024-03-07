/*
	even though in this book
		we are only dealing with spheres
	we will make it ready for other hittable (important)
	sphere_list -> hittable_list
*/


#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>
#include "rtweekend.h"

class hittable_list : public hittable {
	public:
		hittable_list() {}
		hittable_list(shared_ptr<hittable>& obj) { add(obj); }
		// clear the list 
		void clear() { objs.clear(); }
		// add a hittable obj to the list
		void add(shared_ptr<hittable>& obj) { objs.push_back(obj); }


		bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	public:
		// instead of a vec of hittable objs, we'll have a vec of pointers to hittable objs 
		std::vector<shared_ptr<hittable>> objs;	// hittable is abstract class 
};

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const {
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	//loops through the dynamic array, updating that max T value each time 
	for (const auto& obj : objs) {
		if (obj->hit(r, interval(ray_t.min, closest_so_far), rec)) {
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}
	return hit_anything;
}
#endif
