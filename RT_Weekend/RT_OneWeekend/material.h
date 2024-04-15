#ifndef MATERIAL_H
#define MATERIAL_H

// ===================================================================
// This file for Ideal reflection modelling "Lambertian implementation"  
// ===================================================================


//#include "utility.h"

#include "hittable_list.h"
#include "texture.h"

class Material{

public : 
	Material() {}
	virtual ~Material() =default;

	virtual bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered) const = 0;
	virtual color emitted(double u, double v, const point3& p) const {
		return color(0, 0, 0);	// 0 intensity
	}
};


class Lambertian : public Material {
	public:
		Lambertian(const color& a) : albedo(make_shared<Constant_texture>(a)) {}
		Lambertian(shared_ptr<Texture> t) : albedo(t) {}

		//  Scatter incoming light in all directions uniformly
		bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered)
		const override {	
			auto scatter_dir = rec.normal + random_unit_vector();
			
			// Catch degenerate scatter direction
			if (scatter_dir.near_zero())
				scatter_dir = rec.normal;

			scattered = Ray(rec.int_p, scatter_dir);
			attenuation = albedo->texture_value(rec.u, rec.v, rec.int_p);
			return true;
		}	

	private:
		shared_ptr<Texture> albedo;	//  A measure of how well a surface reflects incoming light [0,1]
};

class Metal : public Material {
public:
	Metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool Material::scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered)
		const override {
		Vec3 reflected = reflect(Normalize(r_in.direction()), rec.normal);
		scattered = Ray(rec.int_p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	color albedo;
	double fuzz;
};

class Luminant : public Material {
	public :
		Luminant (shared_ptr<Texture> texture_) : emit(texture_) {}
		Luminant(color& _color) : emit(make_shared<Constant_texture>(_color)) {}
		
		bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered)
		const override {
			return false;	// no reflection 
		}

		color emitted(double u, double v, const Vec3& p)const override {
			return emit->texture_value(u, v, p);
		}
	private :
		shared_ptr<Texture> emit;
};

#endif