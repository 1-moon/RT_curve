
#ifndef HITTABLE
#define HITTABLE

#include "Vec3.h"
// struct contains t(ray dir), p(intersection point), normal(n) 
struct hit_record {
	point3 p;
	Vec3 normal;
	double t;
};

// declare the hit method 
class hittable {
public:
	// signature "const =0" means it's a pure virtual function & 
	// any derived class wull have to define it
	// by this pure virtual member function hittable is an abstract class
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

};
#endif // !HITTABLE