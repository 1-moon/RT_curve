#ifndef HITTABLE
#define HITTABLE

#include"rtweekend.h"

class Material;  // alert the compiler that the pointer is to a class,

class Hit_record {
public:
	point3 int_p;	// intersection point
	Vec3 normal;	// normal at intersection point
	double t;		// parameter value of the ray at intersection point
	shared_ptr<Material> mat_ptr;	// pointer to the material of the object
};

class Hittable {
	public:
		virtual ~Hittable() = default;
		// By this pure virtual member, function hittable is an abstract class 
		virtual bool TestIntersection(const Ray& r, interval ray_t, Hit_record& rec) const = 0;

};

#endif 
