
#ifndef BEZIERSURFACE_H 
#define BEZIERSURFACE_H

#include "bezierCurve.h"
#include "boundingBox.h"
#include "triangle.h"
class BezierSurface : public Hittable {
public:
    BezierSurface() : ctrl_points_(nullptr) {}
    BezierSurface(point3* controlPoints, shared_ptr<Material> mat)
        : ctrl_points_(controlPoints), material_(mat) {}

    
    point3 cubicPolynomial(const point3* P, const double& t) const;  

    point3 BezierSurfacePoint(const point3* controlPoints, const double& u, const double& v)const;


    virtual bool TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const ;  

public:
    point3* ctrl_points_;
    shared_ptr<Material> material_;
    double width = 1.0f;
};

point3 BezierSurface::cubicPolynomial(const point3* ctrl_p, const double& t)const {
    double b0 = bernsteinTerm(0, 3, t);
    double b1 = bernsteinTerm(1, 3, t);
    double b2 = bernsteinTerm(2, 3, t);
    double b3 = bernsteinTerm(3, 3, t);

    return ctrl_p[0] * b0 + ctrl_p[1] * b1 + ctrl_p[2] * b2 + ctrl_p[3] * b3;
}

point3 BezierSurface::BezierSurfacePoint(const point3* controlPoints, const double& u, const double& v)const{
    point3 uCurve[4], vCurve[4];
    // u 방향으로 cubic polynomial 계산
    for (int i = 0; i < 4; ++i) {
        point3 ctrl_p[4] = { controlPoints[i], controlPoints[4 + i], controlPoints[8 + i], controlPoints[12 + i] };
        uCurve[i] = cubicPolynomial(ctrl_p, u);
    }
    // 계산된 점들을 사용하여 v 방향으로 cubic polynomial 계산
    for (int i = 0; i < 4; ++i) {
        vCurve[i] = cubicPolynomial(uCurve, v);
    }
    return vCurve[0]; // 이는 surface 상의 점을 나타냅니다.
}


//bool BezierSurface::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
//    bool hit_anything = false;
//    double closest_so_far = ray_t.max;
//
//    // surface tessellation
//    const int divisions = 10; // how many? 
//
//    for (int u = 0; u < divisions; ++u) {
//        for (int v = 0; v < divisions; ++v) {
//            double u_parametre = double(u) / (divisions - 1);
//            double v_parametre = double(v) / (divisions - 1);
//            
//            auto surface_point = BezierSurfacePoint(ctrl_points_, u_parametre, v_parametre);
//            // 각 샘플 포인트 주변에 설정된 가상의 구와 광선의 교차 검사
//            Vec3 oc = castRay.origin() - surface_point;
//            auto a = castRay.direction().length_squared();
//            auto half_b = dot(oc, castRay.direction());
//            auto c = oc.length_squared() - 0.1; // 작은 구의 반지름 설정
//
//            auto discriminant = half_b * half_b - a * c;
//            if (discriminant > 0) {
//                auto sqrtd = sqrt(discriminant);
//
//                // 두 교차점 중 더 가까운 점 찾기
//                auto root = (-half_b - sqrtd) / a;
//                if (root < ray_t.min || root > closest_so_far) {
//                    root = (-half_b + sqrtd) / a;
//                }
//                if (root >= ray_t.min && root <= closest_so_far) {
//                    // 교차 정보 업데이트
//                    rec.t = root;
//                    rec.int_p = castRay.at(rec.t);
//                    rec.normal = Normalize(rec.int_p - surface_point); // 교차점에서의 법선
//                    rec.mat_ptr = material_;
//
//                    hit_anything = true;
//                    closest_so_far = rec.t;
//                }
//            }
//        }
//    }
//
//    return hit_anything;
//}
bool BezierSurface::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    // Surface tessellation
    const int divisions = 10; // Tessellation resolution
    std::vector<point3> vertices; // Store vertices of the tessellated surface

    // Iterate over the tessellated surface and generate vertices
    for (int u = 0; u <= divisions; ++u) {
        for (int v = 0; v <= divisions; ++v) {
            double u_parametre = static_cast<double>(u) / divisions;
            double v_parametre = static_cast<double>(v) / divisions;
            auto surface_point = BezierSurfacePoint(ctrl_points_, u_parametre, v_parametre);
            vertices.push_back(surface_point);
        }
    }

    // Generate triangle mesh from vertices
    for (int u = 0; u < divisions; ++u) {
        for (int v = 0; v < divisions; ++v) {
            int idx = u * (divisions + 1) + v;
            Triangle tri(vertices[idx], vertices[idx + 1], vertices[idx + divisions + 1], material_);
            if (tri.TestIntersection(castRay, ray_t, rec)) {
                if (rec.t < closest_so_far) {
                    closest_so_far = rec.t;
                    hit_anything = true;
                }
            }

            Triangle tri2(vertices[idx + 1], vertices[idx + divisions + 2], vertices[idx + divisions + 1], material_);
            if (tri2.TestIntersection(castRay, ray_t, rec)) {
                if (rec.t < closest_so_far) {
                    closest_so_far = rec.t;
                    hit_anything = true;
                }
            }
        }
    }

    return hit_anything;
}

#endif  ;
