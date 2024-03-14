

#include "sphere.h"

bool Sphere::TestIntersection(const Ray& r, interval ray_t, Hit_record& rec) const {

	Vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c; // discriminant
	if (discriminant < 0) return false;

	auto sqrt_d = sqrt(discriminant);

	//Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrt_d) / a;

	if (!ray_t.surrounds(root)) return false;
	// update the output parameter rec(record)
	rec.t = root;
	rec.int_p = r.at(rec.t);
	rec.normal = (rec.int_p - center) / radius;
	return true;
}