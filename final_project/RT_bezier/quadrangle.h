#ifndef QUADRANGLE_H
#define QUADRANGLE_H
#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"

constexpr double Epsilon = 1e-6;

class Quadrangle : public Hittable {
public:
	Quadrangle(const point3& _vertex, const Vec3& _u, const Vec3& _v, shared_ptr<Material> mat)
		: vertex(_vertex), u(_u), v(_v), material(mat) {
	
		auto normal_vec = cross(u, v); 
		unit_normal = Normalize(normal_vec);
		// D is the distance from the origin to the plane.
		d = dot(unit_normal, vertex);
		// w is the unit normal vector of the plane.
		w = normal_vec / dot(normal_vec, normal_vec);  
	}


	bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const override {

		// Hit ray parallel to the plane ? 
		auto denom = dot(unit_normal, r.direction());
		// Return false if the ray is parallel to the plane.
		if (fabs(denom) < Epsilon) return false;


		// Return false if the hit point parameter t is outside the ray interval.
		auto t = (d - dot(unit_normal, r.origin())) / denom;
		if (!ray_t.contains(t))
			return false;

		// Determine the hit point lies within the planar shape using its plane coordinates.
		auto intersection = r.at(t);
		Vec3 planar_hitpt_vector = intersection - vertex;
		auto alpha = dot(w, cross(planar_hitpt_vector, v));
		auto beta = dot(w, cross(u, planar_hitpt_vector));
		// Return false if the hit point is outside the 2D shape.
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
		double d;
		Vec3 w;

};

#endif // !QUADRANGLE_H
