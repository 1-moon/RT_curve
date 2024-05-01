#ifndef RAYH
#define RAYH
#include "vec3.h"

class Ray {
	public:
		Ray() {}
		Ray(const point3& start_p, const Vec3& dir): _origin(start_p), _direction(dir){}

		Vec3 origin() const { return _origin; }
		Vec3 direction() const { return _direction; }
		Vec3 at(double t) const {
			return _origin + t * _direction;
		}
	public:
		point3 _origin;
		Vec3 _direction;
};
#endif