/*
**	Header file for Vec3 class 
*/

#ifndef VEC3_H	// prevent multiple inclusion
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3{
public:
	// default constructor
	Vec3() : e{0,0,0} {}
	// Intialize the vector with 3 double values
	Vec3(double e0, double e1, double e2): e{e0, e1, e2} {}
	// For accessing member variable of instance   
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	// Vec3 neg_v = -v; -> apply negation to each element of the vector
	inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	// Vec3 v(1,2,3); double x = v[0]; -> access the element of the vector)
	inline double operator[](int i) const { return e[i]; }
	// obj[0]= 20.0; double& ref = obj[0]; -> Accessing first element using [] with & 
	// ref = 30.0; -> modifying through reference
	inline double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	// divide each element of the vector by t(scalar)
	Vec3& operator/=(const double t) {
		return *this *= 1/t;
	}
	double length() const {
		return sqrt(length_squared());
	}
	double length_squared() const {
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	}

public: 
	double e[3];

};

//// utility functions that take vec3 objects 
// output the vector to the output stream
std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
Vec3 operator*(double t, const Vec3& v) {
	return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}
Vec3 operator*(const Vec3& v, double t) {
	return t * v;
}
Vec3 operator/(Vec3 v, double t) {
	return v * (1 / t);
}
// dot product of two vectors
double dot(const Vec3& u, const Vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}
// cross product of two vectors
Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
// unit vector of a vector
Vec3 unit_vector(Vec3 v) {
	return v / v.length();
}

/*	Type alias for Vec3.
	point and color can be used interchangeably with Vec3 !!! 
	many operations are the same for points colour and vec3 
	like addition, subtraction, multiplication, division
*/ 
using point3 = Vec3;   // 3D point
using color = Vec3;    // RGB color

#endif

