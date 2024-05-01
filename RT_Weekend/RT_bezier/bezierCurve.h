#ifndef BEZIERCURVEH
#define BEZIERCURVEH


#include "hittable.h"
#include "utility.h"
#include <vector>
#include <cassert>
#include "triangle.h"
#include "Quadrangle.h"
class BezierCurve : public Hittable
{
public:
	std::vector<point3> ctrl_points;
	std::vector<point3> curve_points;
	shared_ptr<Material> material;
	double width = 10.3f;
public:
	// Default constructor
	BezierCurve(){};
	// Constructor with 4 control points
	BezierCurve(const point3& p0, const point3& p1, const point3& p2, const point3& p3, shared_ptr<Material> m)
	{
		// Clear the control points and curve points
		ctrl_points.clear();
		curve_points.clear();
		material = m;
		// Add the control points
		ctrl_points.push_back(p0);
		ctrl_points.push_back(p1);
		ctrl_points.push_back(p2);
		ctrl_points.push_back(p3);
		// Calculate the curve points
		de_casteljau();
	};

	void add(const point3& ctrl_p) { ctrl_points.push_back(ctrl_p); };
	void de_casteljau();
	point3 de_casteljau_recur(const std::vector<point3>, int i, double t, int k) const;
	double bernstein(int i, int n, double t) const;
	virtual bool TestIntersection(const Ray &castRay, Interval ray_t, Hit_record &rec) const;

};

point3 BezierCurve::de_casteljau_recur(std::vector<point3> ctrl_points_, int i, double t, int k) const
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

double bernsteinTerm(int i, int n, double t)	// n = degree, i = index, t = parameter
{
	// Check valid parameter t [0,1] ?  
	assert(t >= 0.0 && t <= 1.0);
	double multiplier = 1.0;	//	intial coefficient  

	for (int k = 0; k < i; ++k)
		// Caculate binomial coefficient
		multiplier = multiplier * t * (n - k) / (i - k);
	// Calculate the Bernstein polynomial 
	return multiplier * pow(1 - t, n - i);
}

bool BezierCurve::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
	bool hit_anything = false;
	double closest_so_far = ray_t.max;

	// =========================quadratic intersection=========================
	for (int i = 0; i < curve_points.size() - 1; i++) {
		// Calculate the normal at the current point on the curve
		Vec3 curve_tangent = curve_points[i + 1] - curve_points[i];
		Vec3 arbitrary_normal(0, 1, 0); // Default normal for simplicity
		if (curve_tangent.length() > 0) {
			arbitrary_normal = Normalize(cross(curve_tangent, Vec3(0, 0, 1))); // Assuming Z is up
		}

		Vec3 curve_normal = Normalize(cross(curve_tangent, arbitrary_normal));
		Vec3 width_offset = curve_normal * (width / 2.0);

		// Define the vertices of the quad
		point3 v0 = curve_points[i] - width_offset;
		point3 v1 = curve_points[i] + width_offset;
		point3 v2 = curve_points[i + 1] - width_offset;
		point3 v3 = curve_points[i + 1] + width_offset;

		// Create quad objects
		Quadrangle quad1(v0, v1 - v0, v3 - v1, material); // Using two sides as the vector directions
		Quadrangle quad2(v0, v3 - v0, v2 - v0, material); // Second part of quad to cover entire face

		// Test intersection with the first part of the quad
		if (quad1.TestIntersection(castRay, ray_t, rec) && rec.t < closest_so_far) {
			hit_anything = true;
			closest_so_far = rec.t;
		}

		// Test intersection with the second part of the quad
		if (quad2.TestIntersection(castRay, ray_t, rec) && rec.t < closest_so_far) {
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}


	// =========================triangle intersection=========================
	for (int i = 0; i < curve_points.size() - 1; i++) {
		Vec3 tangent = curve_points[i + 1] - curve_points[i]; // 접선 벡터 계산
		Vec3 normalized_tangent = Normalize(tangent);

		// 법선 벡터를 구하는 간단한 방법은 Y축 또는 Z축과 외적을 구하는 것입니다.
		// (실제 사용법에 따라 다른 축을 사용할 수도 있습니다.)
		Vec3 arbitrary_vector = (fabs(normalized_tangent.y()) > 0.9) ? Vec3(0, 0, 1) : Vec3(0, 1, 0);
		Vec3 curve_normal = Normalize(cross(normalized_tangent, arbitrary_vector));
		Vec3 offset = Normalize(curve_normal) * (width / 2.0);

		// Create two triangles to represent the curve's width
		point3 v0 = curve_points[i] - offset;
		point3 v1 = curve_points[i] + offset;
		point3 v2 = curve_points[i + 1] - offset;
		point3 v3 = curve_points[i + 1] + offset;

		// Create triangle objects
		Triangle tri1(v0, v1, v2, material);
		Triangle tri2(v1, v2, v3, material);
		// Test intersection with the first triangle
		if (tri1.TestIntersection(castRay, ray_t, rec) && rec.t < closest_so_far) {
			hit_anything = true;
			closest_so_far = rec.t;
		}

		// Test intersection with the second triangle
		if (tri2.TestIntersection(castRay, ray_t, rec) && rec.t < closest_so_far) {
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}

	// ======================sphere intersection=========================
	for (const auto& curve_point : curve_points) {
		Vec3 oc = castRay.origin() - curve_point; 
		auto a = dot(castRay.direction(), castRay.direction());
		auto half_b = dot(oc, castRay.direction());
		auto c = oc.length_squared() - width * width;

		// Calculate the discriminant
		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) continue; 

		auto sqrtd = sqrt(discriminant);
		auto root = (-half_b - sqrtd) / a;  

		// Check if the root is within the interval
		if (root < ray_t.min || root > closest_so_far) {
			root = (-half_b + sqrtd) / a;
			if (root < ray_t.min || root > closest_so_far) continue;
		}

		// update the hit record
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