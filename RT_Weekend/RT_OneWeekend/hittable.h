#ifndef HITTABLE
#define HITTABLE


class Hittable {
	public:
		// signature "const =0" means it's a pure virtual function & 
		// any derived class will have to define it
		// by this pure virtual member function hittable is an abstract class 
		virtual bool TestIntersection(const Ray& r, interval ray_t, Hit_record& rec) const = 0;

};

class Hit_record {
	public:
		point3 int_p;	// intersection point
		Vec3 normal;
		double t; 
};




#endif 
