
/*
	Axis Aligned Bounding box  a.k.a 'AABB'
	Draw a box around the object to check if the ray intersects the box or not
*/

#ifndef AABBH
#define AABBH

#include "utility.h"

class Aabb {
public:	
	Aabb() {}
	Aabb(const Interval& ix, const Interval& iy ,const Interval& iz)
	: x_interval(ix), y_interval(iy), z_interval(iz){}
	

	Aabb(const point3& a, const point3& b) {
		// Treat the two points a and b as extrema for  the bounding box.
		x_interval = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
		y_interval = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
		z_interval = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
	}

	Aabb(const Aabb& a, const Aabb& b) {
		x_interval = Interval(a.x_interval, b.x_interval);
		y_interval = Interval(a.y_interval, b.y_interval);
		z_interval = Interval(a.z_interval, b.z_interval);
	}


	Aabb& add(const point3& new_vertex) {
		x_interval = Interval(fmin(x_interval.min, new_vertex[0]), fmax(x_interval.max, new_vertex[0]) );
		y_interval = Interval(fmin(y_interval.min, new_vertex[1]), fmax(y_interval.max, new_vertex[1]) );
		z_interval = Interval(fmin(z_interval.min, new_vertex[2]), fmax(z_interval.max, new_vertex[2]) );
		return *this;
	}


	Aabb& Aabb::add(const Aabb& other_box) {
		x_interval = Interval(fmin(x_interval.min, other_box.x_interval.min), fmax(x_interval.max, other_box.x_interval.max));
		y_interval = Interval(fmin(y_interval.min, other_box.y_interval.min), fmax(y_interval.max, other_box.y_interval.max));
		z_interval = Interval(fmin(z_interval.min, other_box.z_interval.min), fmax(z_interval.max, other_box.z_interval.max));
		return *this;
	}
	Aabb pad() {
		// Return an AABB that has no side narrower than some delta, padding if necessary.
		double delta = 0.0001;
		Interval new_x = (x_interval.size() >= delta) ? x_interval : x_interval.expand(delta);
		Interval new_y = (y_interval.size() >= delta) ? y_interval : y_interval.expand(delta);
		Interval new_z = (z_interval.size() >= delta) ? z_interval : z_interval.expand(delta);

		return Aabb(new_x, new_y, new_z);
	}

	const Interval& axis(int n) const {
		if (n == 1) return y_interval;
		if (n == 2) return z_interval;
		return x_interval;
	}

	// Return true if the ray intersects the bounding box.
	bool hit(const Ray& r, Interval ray_t) const {

		for (int i = 0; i < 3; i++) {
			auto invD = 1 / r.direction()[i];
			auto orig = r.origin()[i];

			auto t0 = (axis(i).min - orig) * invD;
			auto t1 = (axis(i).max - orig) * invD;

			if (invD < 0)
				std::swap(t0, t1);

			if (t0 > ray_t.min) ray_t.min = t0;
			if (t1 < ray_t.max) ray_t.max = t1;

			if (ray_t.max <= ray_t.min)
				return false;
		}
		return true;
	}

public: 
	Interval x_interval, y_interval, z_interval;

};

Aabb operator+(const Aabb& bbox, const Vec3& offset) {
	return Aabb(bbox.x_interval + offset.x(), bbox.y_interval + offset.y(), bbox.z_interval + offset.z());
}

Aabb operator+(const Vec3& offset, const Aabb& bbox) {
	return bbox + offset;
}

#endif 
