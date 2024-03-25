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
#include "utility.h"
#include <memory>


class Hittable_list : public Hittable {
	public:
		// The constructor 
		Hittable_list() {}
		Hittable_list(shared_ptr<Hittable>& obj) { add(obj); }
		
		// Clear the list 
		void clear() { objs.clear(); }
		
		// Add a hittable obj to the list
		//void add(shared_ptr<Hittable>& obj) { objs.push_back(obj); }
		void add(const std::shared_ptr<Hittable>& obj) { objs.push_back(obj); }

		// Function to test for intersections.
		bool Hittable::TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const override {
			bool hit_anything = false;
			auto closest_so_far = ray_t.max;

			//loops through the dynamic array, updating that max T value each time 
			for (const auto& obj : objs) {
				if (obj->TestIntersection(r, Interval(ray_t.min, closest_so_far), rec)) {
					hit_anything = true;
					closest_so_far = rec.t;
				}
			}
			return hit_anything;
		}


	public:
		// instead of a vec of hittable objs, 
		// we'll have a vec of pointers to hittable objs 
		std::vector<shared_ptr<Hittable>> objs;	// hittable is abstract class 
};



#endif
