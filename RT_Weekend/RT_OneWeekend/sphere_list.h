 #ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include "sphere.h"
#include "hittable.h"

#include <vector>


class sphere_list {
	public: 
		sphere_list() {}
		sphere_list(sphere obj) {add(obj);}

		// add a sphere to the list
		void add(sphere obj) { objs.push_back(obj); }		
		// clear the list 
		void clear() { objs.clear(); }
		
		bool hit (const ray& r, double t_min, double t_max, hit_record& rec) const;

	public:
		// dynamic array of spheres
		std::vector<sphere> objs;
};
 
bool sphere_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	bool hit_anything = false;
	auto closest_so_far = t_max;
	//loops through the dynamic array, updating that max T value each time 
	for (const auto& obj : objs) {
		if (obj.hit(r, t_min, closest_so_far, rec)) {	// object being a sphere 
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}
	return hit_anything;
}
#endif