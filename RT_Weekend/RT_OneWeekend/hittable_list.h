/*
	even though in this book
		we are only dealing with spheres
	we will make it ready for other hittable 
*/


#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include "hittable.h"

#include <vector>

// we will drive from the base calss 
class hittable_list : public hittable {
public:
	hittable_list() {}
	hittable_list(hittable& obj) { add(obj); }

	// add a sphere to the list
	void add(const hittable& obj) { objs.push_back(&obj); }
	// clear the list 
	void clear() { objs.clear(); }

	bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	// dynamic array of spheres
	// c++ vectros work by knowing how much space to allocate when new objs are added to it 
	// i.e. abstract claass can not work, so a vector of pointers to hittable objects is used
	std::vector<const hittable*> objs;	// hittable is abstract class 
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	bool hit_anything = false;
	auto closest_so_far = t_max;

	//loops through the dynamic array, updating that max T value each time 
	for (const auto& obj : objs) {
		if (obj->hit(r, t_min, closest_so_far, rec)) {
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}
	return hit_anything;
}
#endif