#ifndef HITTABLE
#define HITTABLE


class hit_record {
	public:
		point3 p;
		Vec3 normal;
		double t;
};

// declare the hit method 
class hittable {
	public:
		// signature "const =0" means it's a pure virtual function & 
		// any derived class will have to define it
		// by this pure virtual member function hittable is an abstract class 
		virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};


#endif 
