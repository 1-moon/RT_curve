#ifndef TEXTUREH 
#define TEXTUREH
#include "vec3.h"

class Texture {
	public :
		virtual ~Texture() {}
		virtual color texture_value (double u, double v, const point3& p) const = 0;

};

class Constant_texture : public Texture {
	public:
		Constant_texture() {}
		Constant_texture(color c) : color_value(c) {}

		virtual color texture_value(double u, double v, const point3& p) const override {
			return color_value;
	}

	private:
		color color_value;
};

class Checker_texture : public Texture {
	public:
		Checker_texture() {}
		Checker_texture(color c1, color c2) 
			: even(make_shared<Constant_texture>(c1)), odd(make_shared<Constant_texture>(c2)) {}
		Checker_texture(shared_ptr<Texture> t0, shared_ptr<Texture> t1)
			: even(t0), odd(t1) {}

		virtual color texture_value(double u, double v, const point3& p) const override {
			auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
			if (sines < 0)
				return odd->texture_value(u, v, p);
			else
				return even->texture_value(u, v, p);
		}

	private:
		shared_ptr<Texture> odd;
		shared_ptr<Texture> even;
};

#endif 
