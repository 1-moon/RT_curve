#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// derive sphere from the hittable class
class sphere : public hittable{
public:
	sphere() {};
	sphere(point3 cen, double r) : centre(cen), radius(r) {};
	// function hit , override the abstact class  
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public: // Sphere class member 
	point3 centre;
	double radius;

};

//class sphere {
//	public:
//		sphere() {};
//		sphere(point3 cen, double r) : centre (cen), radius (r) {};
//		// function hit 
//		bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const; 
//
//	public : // Sphere class member 
//		point3 centre;
//		double radius;   
//
//};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {	// psss in a range 
	// similar to the hit_sphere function in chapter 5 -> but, it returns boolean
	Vec3 oc = r.origin() - centre; 
	auto a = r.direction().length_squared();  
	auto half_b = dot(oc, r.direction());	 // even number 
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c; // discriminant
	if (discriminant < 0) return false;
	
	auto sqrt_d = sqrt(discriminant);

	//Find the nearest root that lies in the acceptable range.
	auto root = (-half_b-sqrt_d) / a;

	if (root < t_min || t_max < root) return false;
	// update the output parameter rec(record)
	rec.t = root;
	rec.p = r.at(rec.t);
	rec.normal = (rec.p - centre) / radius;
	return true;
}
	

#endif