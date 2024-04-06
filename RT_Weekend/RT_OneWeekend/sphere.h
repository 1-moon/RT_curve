#ifndef SPHERE_H
#define SPHERE_H
#include "utility.h"

//#include "hittable.h"

// derive sphere from the hittable class
class Sphere : public Hittable{
public:
	Sphere(point3 cen, double r, shared_ptr<Material> m)
		: center(cen), radius(r), material(m) {};
	// function hit , override the abstact class  
	virtual bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const;
	
	bool TestIntersectionCSG(const Ray& r, const Sphere& sphere1, const Sphere& sphere2, Hit_record& rec) const;
public: // Sphere class member 
	point3 center;
	double radius;
	shared_ptr<Material> material;
};

bool Sphere::TestIntersection(const Ray& cameraRay, Interval ray_t, Hit_record& rec) const {	 
	
	Vec3 oc = cameraRay.origin() - center;
	auto a = cameraRay.direction().length_squared();
	auto half_b = dot(oc, cameraRay.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c; // discriminant
	if (discriminant < 0) return false;
	
	auto sqrt_d = sqrt(discriminant);

	//Find the nearest root that lies in the acceptable range.
	auto root = (-half_b-sqrt_d) / a;

	if (!ray_t.surrounds(root)) return false;
	// update the output parameter rec(record)
	rec.t = root;
	rec.int_p = cameraRay.at(rec.t);
	rec.normal = (rec.int_p - center) / radius;
	rec.mat_ptr= material;
	return true;

}

bool Sphere::TestIntersectionCSG(const Ray& r, const Sphere& sphere1, const Sphere& sphere2, Hit_record& rec) const {
	Hit_record rec_1, rec_2;

	// Test intersection with each spheres 
	bool hit_1 = sphere1.TestIntersection(r, Interval(0.0001, infinity), rec_1);
	bool hit_2 = sphere2.TestIntersection(r, Interval(0.0001, infinity), rec_2);

	// CSG Intersection Operation: only return true if both spheres are hit
	if (hit_1 && hit_2) {
		// Choose the closer of the two intersection points
		rec = (rec_1.t < rec_2.t) ? rec_1 : rec_2;
		return true;
	}

	return false; // 교차점이 없습니다.
}

#endif