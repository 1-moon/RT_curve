#ifndef SPHERE_H
#define SPHERE_H
#include "utility.h"

#include "hittable.h"

// derive sphere from the hittable class
class Sphere : public Hittable{
public:
	Sphere(point3 cen, double r, shared_ptr<Material> m)
		: center(cen), radius(r), material(m) {
		// Generate the bounding box for a sphere 
		auto radius_vec = Vec3(radius, radius, radius);
		bBox = Aabb(center - radius_vec, center + radius_vec);
	};

	// function hit , override the abstact class  
	virtual bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const;
	Aabb BoundingBox() const override { return bBox; }

private: // Sphere class member 
	point3 center;
	double radius;
	shared_ptr<Material> material;
	Aabb bBox;
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
	rec.set_face_normal(cameraRay, rec.normal);
	rec.mat_ptr= material;
	return true;

}

#endif