#ifndef TEXTUREH 
#define TEXTUREH
#include "vec3.h"

class Texture {
	public :
		virtual ~Texture() = default;
		virtual color texture_value (double u, double v, const point3& p) const = 0;

};

class Constant_texture : public Texture {
	public:
		Constant_texture() {}
		Constant_texture(color c) : color_value(c) {}
		Constant_texture(double red, double green, double blue)
			: Constant_texture(color(red, green, blue)) {}

		virtual color texture_value(double u, double v, const point3& p) const override {
			return color_value;
	}

	private:
		color color_value;
};

#endif 
