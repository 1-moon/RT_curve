#ifndef QUADRATIC_H
#define QUADRATIC_H
#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"


class Quadratic : public Hittable {
public:
	Quadratic(const point3& _vertex, const Vec3& _u, const Vec3& _v, shared_ptr<Material> mat)

		: vertex(_vertex), u(_u), v(_v), material(mat) {
	
		auto normal_vec = cross(u, v); 
		unit_normal = Normalize(normal_vec);
		D = dot(unit_normal, vertex);
		w = normal_vec / dot(normal_vec, normal_vec);  
	}

	//Aabb BoundingBox() const override { return bBox; }

	bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const override {

		// Hit ray parallel to the plane ? 
		auto denom = dot(unit_normal, r.direction());
		if (fabs(denom) < 1e-6) return false;


		// Return false if the hit point parameter t is outside the ray interval.
		auto t = (D - dot(unit_normal, r.origin())) / denom;
		if (!ray_t.contains(t))
			return false;

		// Determine the hit point lies within the planar shape using its plane coordinates.
		auto intersection = r.at(t);
		Vec3 planar_hitpt_vector = intersection - vertex;
		auto alpha = dot(w, cross(planar_hitpt_vector, v));
		auto beta = dot(w, cross(u, planar_hitpt_vector));

		if (!is_interior(alpha, beta, rec))
			return false;

		// Ray hits the 2D shape; set the rest of the hit record and return true.
		rec.t = t;
		rec.int_p = intersection;
		rec.mat_ptr = material;
		rec.set_face_normal(r, unit_normal);

		return true;
	};

	virtual bool is_interior(double a, double b, Hit_record& rec) const {
		// Given the hit point in plane coordinates, return false if it is outside the
		// primitive, otherwise set the hit record UV coordinates and return true.

		if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}

	private:
		point3 vertex; 
		Vec3 u, v; 
		shared_ptr<Material> material;
		Vec3 unit_normal;
		double D;
		Vec3 w;

};


inline shared_ptr<Hittable_list> box(const point3& a, const point3& b, shared_ptr<Material> mat)
{
	// Returns the 3D box (six sides) that contains the two opposite vertices a & b.

	auto sides = make_shared<Hittable_list>();

	// Construct the two opposite vertices with the minimum and maximum coordinates.
	auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
	auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

	auto dx = Vec3(max.x() - min.x(), 0, 0);
	auto dy = Vec3(0, max.y() - min.y(), 0);
	auto dz = Vec3(0, 0, max.z() - min.z());

	sides->add(make_shared<Quadratic>(point3(min.x(), min.y(), max.z()), dx, dy, mat)); // front
	sides->add(make_shared<Quadratic>(point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
	sides->add(make_shared<Quadratic>(point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
	sides->add(make_shared<Quadratic>(point3(min.x(), min.y(), min.z()), dz, dy, mat)); // left
	sides->add(make_shared<Quadratic>(point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
	sides->add(make_shared<Quadratic>(point3(min.x(), min.y(), min.z()), dx, dz, mat)); // bottom

	return sides;
}
#endif // !QUADRATIC_H
