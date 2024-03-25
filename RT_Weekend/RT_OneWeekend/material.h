#ifndef MATERIAL_H
#define MATERIAL_H

// ===================================================================
// This file for Ideal reflection modelling "Lambertian implementation"  
// ===================================================================


//#include "utility.h"

#include "hittable_list.h"

class Material{

public : 
	Material() {}
	virtual ~Material() =default;

	virtual bool scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered) const = 0;
};


class Lambertian : public Material {
	public:
		Lambertian(const color& a) : albedo(a) {}
		//  Scatter incoming light in all directions uniformly
		bool Material::scatter(const Ray& r_in, const Hit_record& rec, color& attenuation, Ray& scattered) const override {
		
			auto scatter_dir = rec.normal + random_unit_vector();
		
			scattered = Ray(rec.int_p, scatter_dir);
			attenuation = albedo;
			return true;
		}	

	private:
		color albedo;	//  A measure of how well a surface reflects incoming light [0,1]
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

#endif