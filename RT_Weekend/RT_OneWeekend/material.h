#ifndef MATERIAL_H
#define MATERIAL_H

// ===================================================================
// This file for Ideal reflection modelling "Lambertian implementation"  
// ===================================================================


#include "rtweekend.h"

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
#endif