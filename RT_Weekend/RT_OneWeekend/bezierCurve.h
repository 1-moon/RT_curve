#ifndef BEZIERCURVEH
#define BEZIERCURVEH

#define curve_width 0.2f
#include "hittable.h"

class BezierCurve : public Hittable
{
public:
	// Default constructor
	BezierCurve(){};
	// Constructor with 4 control points
	BezierCurve(const point3 p0, const point3 p1, const point3 p2, const point3 p3)
	{
		ctrl_points.clear();
		ctrl_points.push_back(std::make_shared<point3>(p0));
		ctrl_points.push_back(std::make_shared<point3>(p1));
		ctrl_points.push_back(std::make_shared<point3>(p2));
		ctrl_points.push_back(std::make_shared<point3>(p3));
		ctrl_points.push_back(std::make_shared<point3>(-1.0f, 1.0f, -1.0f));
		ctrl_points.push_back(std::make_shared<point3>(1.0f, 1.0f, -1.0f));
		ctrl_points.push_back(std::make_shared<point3>(1.0f, 1.0f, 1.0f));
		ctrl_points.push_back(std::make_shared<point3>(-1.0f, 1.0f, 1.0f));
		de_casteljau();
	};

	void clear() { ctrl_points.clear(); }
	void add(const std::shared_ptr<point3> &ctrl_p) { ctrl_points.push_back(ctrl_p); };
	void de_casteljau();
	point3 de_casteljau_recur(const std::vector<shared_ptr<point3>>, int i, double t, int k) const;
	virtual bool TestIntersection(const Ray &castRay, interval ray_t, Hit_record &rec) const;

public:
	std::vector<shared_ptr<point3>> ctrl_points;
	std::vector<shared_ptr<point3>> curve_points;
	//size_t sample_size_;

private:
};
point3 BezierCurve::de_casteljau_recur(std::vector<shared_ptr<point3>> ctrl_points_,
																			 int i, double t, int k) const
{
	if (k == 0)
	{
		return *ctrl_points_[i];
	}
	else
	{
		// P_curve = (1 - t)*i_point + t*(i_point+1)
		return (1 - t) * de_casteljau_recur(ctrl_points_, i, t, k - 1) + t * de_casteljau_recur(ctrl_points_, i + 1, t, k - 1);
	}
}

void BezierCurve::de_casteljau()
{

	auto ctrl_points_size = ctrl_points.size();
	// Initialize the curve points
	curve_points.resize(ctrl_points_size * 50);

	auto curve_points_size = curve_points.size();
	// Define the interval between each point
	double interval = 1.0f / (curve_points_size - 1);

	if (ctrl_points.size() == 0)
	{
		std::cerr << "No control points" << std::endl;
		return;
	}
	else if (ctrl_points.size() == 1)
	{
		// Curve is a point
		for (auto &i : curve_points)
		{
			i = ctrl_points[0];
		}
	}
	else if (ctrl_points.size() == 2)
	{
		// Linear curve
		for (auto i = 0; i < curve_points_size; ++i)
		{
			// Linear interpolation ex) P01 = (1 - t)P0 + tP1
			double t = i * interval;
			curve_points[i] = std::make_shared<point3>(*(ctrl_points[0]) * (1 - t) + *(ctrl_points[1]) * t);
		}
	}
	else
	{
		// De Casteljau's algorithm
		for (auto i = 0; i < curve_points_size; ++i)
		{
			double t = i * interval;
			curve_points[i] = std::make_shared<point3>(de_casteljau_recur(ctrl_points,0, t, static_cast<int>(ctrl_points_size) - 1));
		}
	}
}


bool BezierCurve::TestIntersection(const Ray &castRay, interval ray_t, Hit_record &rec) const
{
	bool hit_anything = false; 

	 // extract each segment of the curve 
	for (auto i = 0; i < curve_points.size() - 1; i++) {
		point3 start = *curve_points[i];	
		point3 end = *curve_points[i + 1];
	
		Vec3 v = end - start; // segment direction vector 
		Vec3 w = castRay.origin() - start; 

		Vec3 v_cross_d = cross (v, castRay.direction());

		if (v_cross_d.length_squared() < 1e-8) {
			// ray is parallel to the segment
			continue; 
		}

		double s = dot(cross(w, castRay.direction()), v_cross_d) / v_cross_d.length_squared();
		if (s < 0 || s >1) {
			// intersection point is outside the segment
			continue;
		}

		double t = dot(cross(w, v), v_cross_d) / v_cross_d.length_squared();
		if (t < 0 || !ray_t.surrounds(t)) {
			// interaction point is at behind the ray or outside the interval
			continue;
		}

		if (t < rec.t) {
			hit_anything = true;
			rec.t = t;
			rec.int_p = castRay.at(t);	// intersection point

			// normal vector 
			Vec3 segment_normal = Normalize(cross(v, cross(v, castRay.direction())));
			rec.normal = cross(segment_normal, v);
		}
		
		
	}
	return hit_anything;
}



#endif