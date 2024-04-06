
/*
	Axis Aligned Bounding box 
	Draw a box around the object to check if the ray intersects the box or not
*/

#ifndef BOUNDINGBOXH
#define BOUNDINGBOXH

#include "utility.h"

class BoundingBox {
public:	
	BoundingBox() {}
	BoundingBox(const Interval& ix, const Interval& iy ,const Interval& iz)
	: x_interval(ix), y_interval(iy), z_interval(iz){}
	BoundingBox(const point3& v_min, const point3& v_max) {}

	BoundingBox& add(const point3& new_vertex) {
		x_interval = Interval(fmin(x_interval.min, new_vertex[0]), fmax(x_interval.max, new_vertex[0]) );
		y_interval = Interval(fmin(y_interval.min, new_vertex[1]), fmax(y_interval.max, new_vertex[1]) );
		z_interval = Interval(fmin(z_interval.min, new_vertex[2]), fmax(z_interval.max, new_vertex[2]) );
		return *this;
	}

	BoundingBox& BoundingBox::add(const BoundingBox& other_box) {
		x_interval = Interval(fmin(x_interval.min, other_box.x_interval.min), fmax(x_interval.max, other_box.x_interval.max));
		y_interval = Interval(fmin(y_interval.min, other_box.y_interval.min), fmax(y_interval.max, other_box.y_interval.max));
		z_interval = Interval(fmin(z_interval.min, other_box.z_interval.min), fmax(z_interval.max, other_box.z_interval.max));
		return *this;
	}

private: 
	Interval x_interval, y_interval, z_interval;

};



#endif 
