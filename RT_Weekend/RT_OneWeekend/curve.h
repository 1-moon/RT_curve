#ifndef CURVEH
#define CURVEH

#include "hittable.h"

class Curve : public Hittable {
	public:
		Curve() {}
		// Quadratic curve
		Curve(const point3& p0, const point3& p1, const point3& p2) {
			clearControlPoints();
			
			addControlPoint(p0);
			addControlPoint(p1);
			addControlPoint(p2);
		}
		// Qubic curve 
		Curve(const point3& p0, const point3& p1, const point3& p2, const point3& p3) {
			clearControlPoints();
			
			addControlPoint(p0);
			addControlPoint(p1);
			addControlPoint(p2);
			addControlPoint(p3);
		}

		virtual bool TestIntersection(const Ray& castRay, interval ray_t, Hit_record& rec) const;
		
	public:
		void addControlPoint(const point3& p) { controlPoints.push_back(make_shared<point3>(p)); };
		void clearControlPoints() { controlPoints.clear(); }
	private:
		std::vector<shared_ptr<point3>> controlPoints;
};

bool Curve::TestIntersection(const Ray& castRay, interval ray_t, Hit_record& rec) const {
	// try to make a triangle from the control points 


	return false;
}

#endif