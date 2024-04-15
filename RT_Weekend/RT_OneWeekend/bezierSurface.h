
#ifndef BEZIERSURFACE_H 
#define BEZIERSURFACE_H

#include "bezierCurve.h"
#include "aabb.h"
#include "triangle.h"


class BezierSurface : public Hittable {
public:
    BezierSurface()  {};
    BezierSurface(const std::vector<point3>& controlPoints, shared_ptr<Material> mat)
        : ctrl_points(controlPoints), material(mat) {
        calculateBoundingBox();
        tessellateSurface();
    }

    point3 cubicPolynomial(const std::vector<point3>& P, double t) const;
    point3 BezierSurfacePoint(double u, double v) const;
    virtual bool TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const;

    Aabb BoundingBox() const override {
        return bBox;
    }
private: 
    void calculateBoundingBox() {
        // empty bounding box
        if (ctrl_points.empty())
            return;
        // initialize bounding box data
        point3 min_point = ctrl_points[0];
        point3 max_point = ctrl_points[0];
        // find min and max points
        for (size_t i = 1; i < ctrl_points.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                min_point[j] = std::min(min_point[j], ctrl_points[i][j]);
                max_point[j] = std::max(max_point[j], ctrl_points[i][j]);
            }
        }
        bBox = Aabb(min_point, max_point);
    }
    void BezierSurface::tessellateSurface() {
        const int divisions = 10; // Tessellation resolution
        std::vector<point3> vertices;
        for (int u = 0; u <= divisions; ++u) {
            for (int v = 0; v <= divisions; ++v) {
                double u_param = static_cast<double>(u) / divisions;
                double v_param = static_cast<double>(v) / divisions;
                vertices.push_back(BezierSurfacePoint(u_param, v_param));
            }
        }
        for (int u = 0; u < divisions; ++u) {
            for (int v = 0; v < divisions; ++v) {
                int idx = u * (divisions + 1) + v;
                tessellated_triangles.emplace_back(vertices[idx], vertices[idx + 1], vertices[idx + divisions + 1], material);
                tessellated_triangles.emplace_back(vertices[idx + 1], vertices[idx + divisions + 2], vertices[idx + divisions + 1], material);
            }
        }
    }

public:
    std::vector<point3> ctrl_points;
    std::vector<Triangle> tessellated_triangles;
    shared_ptr<Material> material;
    Aabb bBox;
};

point3 BezierSurface::cubicPolynomial(const std::vector<point3>& P, double t) const {
    int degree = static_cast<int>(P.size()) - 1;
    point3 result(0, 0, 0);
    for (int i = 0; i <= degree; ++i) {
        double coeff = bernsteinTerm(i, degree, t);
        result += P[i] * coeff;
    }
    return result;
}

point3 BezierSurface::BezierSurfacePoint(double u, double v) const {
    std::vector<point3> uCurve(4), vCurve(4);
    for (int i = 0; i < 4; ++i) {
        std::vector<point3> ctrl_p = { ctrl_points[i], ctrl_points[4 + i], ctrl_points[8 + i], ctrl_points[12 + i] };
        uCurve[i] = cubicPolynomial(ctrl_p, u);
    }
    for (int i = 0; i < 4; ++i) {
        vCurve[i] = cubicPolynomial(uCurve, v);
    }
    return vCurve[0];
}

bool BezierSurface::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
    bool hit_anything = false;
    double closest_so_far = ray_t.max;
    for (const auto& tri : tessellated_triangles) {
        if (tri.TestIntersection(castRay, ray_t, rec) && rec.t < closest_so_far) {
            closest_so_far = rec.t;
            hit_anything = true;
        }
    }
    return hit_anything;
}

#endif  ;
