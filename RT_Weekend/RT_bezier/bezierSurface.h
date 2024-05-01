
#ifndef BEZIERSURFACE_H 
#define BEZIERSURFACE_H

#include "bezierCurve.h"
#include "triangle.h"


class BezierSurface : public Hittable {
public:
    BezierSurface()  {};
    BezierSurface(const std::vector<point3>& controlPoints, shared_ptr<Material> mat)
        : ctrl_points(controlPoints), material(mat) {
        tessellate();
    }

    point3 cubicPolynomial(const std::vector<point3>& P, double t) const;
    point3 BezierSurfacePoint(double u, double v) const;
    virtual bool TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const;


private: 
    // Divide the surface into a grid of triangles
    void BezierSurface::tessellate() {
        // number of divisions in each direction
        const int divisions = 10;  // if it goes up, computation goes up as well  
        std::vector<point3> vertices;
            
        // generate a grid of points on the surface
        for (int u = 0; u <= divisions; ++u) {
            for (int v = 0; v <= divisions; ++v) {
                double u_parameter = static_cast<double>(u) / divisions;
                double v_parameter = static_cast<double>(v) / divisions;
                vertices.push_back(BezierSurfacePoint(u_parameter, v_parameter));
            }
        }
        // generate triangles from the grid of points 
        for (int u = 0; u < divisions; ++u) {
            for (int v = 0; v < divisions; ++v) {
                // indices of the four vertices of the quad 
                int idx = u * (divisions + 1) + v;
                // add two triangles to the list
                tessellated_triangles.emplace_back(vertices[idx], vertices[idx + 1],
                    vertices[idx + divisions + 1], material);
                tessellated_triangles.emplace_back(vertices[idx + 1], vertices[idx + divisions + 2],
                    vertices[idx + divisions + 1], material);
            }
        }
    }
public:
    std::vector<point3> ctrl_points;
    std::vector<Triangle> tessellated_triangles;
    shared_ptr<Material> material;

};


point3 BezierSurface::cubicPolynomial(const std::vector<point3>& P, double t) const {  
     
    int degree = static_cast<int>(P.size()) - 1; 
    point3 result(0, 0, 0);
    // Bernstein polynomials 
    for (int i = 0; i <= degree; ++i) {
        double coeff = bernsteinTerm(i, degree, t);
        result += P[i] * coeff;
    }
    return result;
}

point3 BezierSurface::BezierSurfacePoint(double u, double v) const {
    // Bezier surface is defined by 16 control points
    std::vector<point3> uCurve(4), vCurve(4), ctrl_p;
    // Bernstein polynomials for u
    for (int i = 0; i < 4; ++i) {   
        // control points for the i-th curve
        ctrl_p = {ctrl_points[i], ctrl_points[4+i], ctrl_points[8+i], ctrl_points[12+i]};
        uCurve[i] = cubicPolynomial(ctrl_p, u);
    }
    // Bernstein polynomials for v
    for (int i = 0; i < 4; ++i) {
        vCurve[i] = cubicPolynomial(uCurve, v);
    }
    // 
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
