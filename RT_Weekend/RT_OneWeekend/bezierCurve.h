#ifndef BEZIERCURVEH
#define BEZIERCURVEH


#include "hittable.h"

class BezierCurve : public Hittable
{
public:
	// Default constructor
	BezierCurve(){};
	// Constructor with 4 control points
	BezierCurve(const point3& p0, const point3& p1, const point3& p2, const point3& p3, shared_ptr<Material> m)
	{
		ctrl_points.clear();
		curve_points.clear();
		material = m;

		ctrl_points.push_back(p0);
		ctrl_points.push_back(p1);
		ctrl_points.push_back(p2);
		ctrl_points.push_back(p3);

		de_casteljau();
	};

	void add(const point3& ctrl_p) { ctrl_points.push_back(ctrl_p); };
	void de_casteljau();
	point3 de_casteljau_recur(const std::vector<point3>, int i, double t, int k) const;
	virtual bool TestIntersection(const Ray &castRay, Interval ray_t, Hit_record &rec) const;

public:
	std::vector<point3> ctrl_points;
	std::vector<point3> curve_points;
	shared_ptr<Material> material;
	double width=0.1f;

};
point3 BezierCurve::de_casteljau_recur(std::vector<point3> ctrl_points_,
																			 int i, double t, int k) const
{
	if (k == 0){
		return ctrl_points_[i];
	}else{
		// P_curve = (1 - t)*i_point + t*(i_point+1)
		return (1 - t) * de_casteljau_recur(ctrl_points_, i, t, k - 1) + t * de_casteljau_recur(ctrl_points_, i + 1, t, k - 1);
	}
}

void BezierCurve::de_casteljau()
{
	curve_points.clear();

	// Initialize the curve points
	curve_points.resize(ctrl_points.size() * 50);	
	// Define the interval between each point
	double interval = 1.0f / (curve_points.size() - 1);

	// Create the curve points 
	if (ctrl_points.size() == 0){
		std::cerr << "No control points" << std::endl;
		return;
	}else if (ctrl_points.size() == 1){
		// Curve is a point
		
		for (auto &i : curve_points){
			i = ctrl_points[0];
		}

	}else if (ctrl_points.size() == 2){

		// Linear curve
		for (auto i = 0; i < curve_points.size(); ++i)
		{
			// Linear interpolation ex) P01 = (1 - t)P0 + tP1
			double t = i * interval;
			curve_points[i] = point3 (ctrl_points[0] * (1 - t) + ctrl_points[1] * t);
		}
	}else{
		// De Casteljau's algorithm
		for (auto i = 0; i < curve_points.size(); ++i)
		{
			double t = i * interval;
			curve_points[i] = de_casteljau_recur(ctrl_points,0, t, static_cast<int>(ctrl_points.size()) - 1);
		}
	}
}

bool BezierCurve::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
	bool hit_anything = false;
	double closest_so_far = ray_t.max;

	for (const auto& curve_point : curve_points) {
		Vec3 oc = castRay.origin() - curve_point;  
		auto a = dot(castRay.direction(), castRay.direction());
		auto half_b = dot(oc, castRay.direction());
		auto c = oc.length_squared() - width * width;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) continue; 

		auto sqrtd = sqrt(discriminant);
		auto root = (-half_b - sqrtd) / a;  

		
		if (root < ray_t.min || root > closest_so_far) {
			root = (-half_b + sqrtd) / a;
			if (root < ray_t.min || root > closest_so_far) continue;
		}

		
		rec.t = root;
		rec.int_p = castRay.at(rec.t);  
		rec.normal = Normalize(rec.int_p - curve_point);  
		rec.mat_ptr = material;

		hit_anything = true;
		closest_so_far = rec.t;  
	}

	return hit_anything;
}
#endif