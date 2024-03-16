#ifndef BEZIERCURVEH
#define BEZIERCURVEH

#define curve_width 0.2f
#include "hittable.h"


class BezierCurve : public Hittable {
	public:
		// Default constructor
		BezierCurve() {};
		// Constructor with 4 control points
		BezierCurve(const point3 p0, const point3 p1, const point3 p2, const point3 p3) {
			ctrl_points.clear();
			ctrl_points.push_back(std::make_shared<point3>(p0));
			ctrl_points.push_back(std::make_shared<point3>(p1));
			ctrl_points.push_back(std::make_shared<point3>(p2));
			ctrl_points.push_back(std::make_shared<point3>(p3));

			de_casteljau();

		};

		void clear() { ctrl_points.clear(); }
		void add (const std::shared_ptr<point3>& ctrl_p) { ctrl_points.push_back(ctrl_p); };
		void de_casteljau();
		point3 de_casteljau_recur(const std::vector<shared_ptr<point3>>, int i, double t, int k) const;
		virtual bool TestIntersection(const Ray& castRay, interval ray_t, Hit_record& rec) const;
		
		
	public:
		std::vector<shared_ptr<point3>> ctrl_points;	  
		std::vector<shared_ptr<point3>> curve_points;	 

	private:

};	
point3 BezierCurve::de_casteljau_recur(std::vector<shared_ptr<point3>> ctrl_points_,
										int i, double t, int k) const {
	if (k == 0) {
		return *ctrl_points_[i];
	}
	else {
		// P_curve = (1 - t)*i_point + t*(i_point+1)
		return (1 - t) * de_casteljau_recur(ctrl_points_, i, t, k - 1)
			+ t * de_casteljau_recur(ctrl_points_, i + 1, t, k - 1);
	}
}

void BezierCurve::de_casteljau() {
	int ctrl_points_size = ctrl_points.size();
	int curve_points_size = curve_points.size();
	
	// Initialize the curve points
	curve_points.resize(ctrl_points_size * curve_points_size);
	// Define the interval between each point
	double interval = 1.0f / (curve_points.size() - 1);
	
	
	if (ctrl_points.size() == 0) {
		std::cerr << "No control points" << std::endl;
		return;
	}	
	else if (ctrl_points.size() == 1) {
		// Curve is a point
		for (auto& i : curve_points) {
			i = ctrl_points[0];
		}
	}  
	else if (ctrl_points.size() == 2) {
		// Linear curve
		for (int i = 0; i < curve_points_size; ++i) {
			// Linear interpolation ex) P01 = (1 - t)P0 + tP1
			double t = i * interval;
			curve_points[i] = std::make_shared<point3>(*(ctrl_points[0]) * (1 - t) +
				*(ctrl_points[1]) * t);
		}
	}
	else {
		// De Casteljau's algorithm 
		for (int i = 0; i < curve_points_size; ++i)
		{
			double t = i * interval;
			curve_points[i] = std::make_shared<point3>(de_casteljau_recur(ctrl_points,
				0, t, ctrl_points_size - 1));
		}
	}
		

}

bool BezierCurve::TestIntersection(const Ray& castRay, interval ray_t, Hit_record& rec) const {

	bool hit_anything = false;
	Hit_record temp_rec;
	double closest_so_far = ray_t.max;

	// 곡선을 여러 선분으로 근사화
	for (size_t i = 0; i < curve_points.size() - 1; i++) {
		// 선분의 양 끝점
		point3 start = *curve_points[i];
		point3 end = *curve_points[i + 1];

		// 선분과 광선의 교차 검사 로직 (간단한 예시)
		// 실제 구현은 선분과 광선의 교차를 계산하는 더 복잡한 로직이 필요할 수 있음
		if (ray_line_segment_intersection(castRay, start, end, temp_rec) && temp_rec.t < closest_so_far) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}




#endif