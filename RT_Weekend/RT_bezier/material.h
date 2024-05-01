#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable_list.h"
#include "texture.h"

class Material{

public : 
	Material() {}
	virtual ~Material() =default;

	virtual bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered) const = 0;
	virtual color emitted(double u, double v, const point3& p) const {
		return color(0, 0, 0);	
	}
};

// A material that scatters light in all directions uniformly
class Lambertian : public Material {
	public:
		Lambertian(const color& a) : albedo(make_shared<Constant_texture>(a)) {}

		//  Scatter incoming light in all directions uniformly
		bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered)
		const override {
			// Calculate the scatter direction S = P + N + E 
			Vec3 scatter_dir = rec.int_p + rec.normal + random_unit_vector();
			
			// Catch degenerate scatter direction
			if (scatter_dir.near_zero())
				scatter_dir = rec.normal;

			scattered = Ray(rec.int_p, scatter_dir-rec.int_p);
			attenuation = albedo->texture_value(rec.u, rec.v, rec.int_p);
			return true;
		}	

	private:
		shared_ptr<Texture> albedo;	//  A measure of how well a surface reflects incoming light [0,1]
};

// A material that emits light 
class Luminant : public Material {
public:
	Luminant(shared_ptr<Texture> texture_) : emit(texture_) {}
	Luminant(color& _color) : emit(make_shared<Constant_texture>(_color)) {}

	bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered)
		const override {
		return false;	// no reflection 
	}

	color emitted(double u, double v, const Vec3& p)const override {
		return emit->texture_value(u, v, p);
	}
private:
	shared_ptr<Texture> emit;
};

#endif