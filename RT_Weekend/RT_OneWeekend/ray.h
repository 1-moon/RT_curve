#ifndef RAYH
#define RAYH
#include "vec3.h"

class Ray {
	public:
		Ray() {}
		Ray(const Vec3& origin, const Vec3& direction): A(origin), B(direction){}

		Vec3 origin() const { return A; }
		Vec3 direction() const { return B; }
		Vec3 at(double t) const {
			return A + t * B;
		}
	public:
		Vec3 A;
		Vec3 B;
};
#endif