#ifndef MATERIAL_H
#define MATERIAL_H

#include "object.h"

class material {
public:
	virtual bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian :public material {
private:
	color albedo;
public:
	lambertian(color attenuation) :albedo(attenuation) {};
	bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = albedo;
		scattered = ray(rec.p, cosine_sample_hemisphere(rec.normal));
		return true;
	}
};

class metal :public material {
private:
	color albedo;
	double fuzz;
public:
	metal(color attenuation, double fuzz) :albedo(attenuation), fuzz(fuzz < 1 ? fuzz : 1) {};
	bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = albedo;
		scattered = ray(rec.p, unit_vector(reflect(r_in.direction(), rec.normal)) + fuzz * random_on_sphere());
		return dot(rec.normal, scattered.direction()) > 0;
	}
};

class dielectric :public material {
private:
	double refraction_index;
public:
	dielectric(double refraction_index) :refraction_index(refraction_index) {};
	bool scatter(ray& r_in, hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1, 1, 1);
		double effective_ri = rec.front_face ? refraction_index : 1 / refraction_index;
		double theta1 = std::acos(dot(unit_vector(-r_in.direction()), rec.normal));
		if (std::sin(theta1) / effective_ri > 1 || dielectric_reflectance(1, effective_ri, theta1) > random_double())
		{
			scattered = ray(rec.p, unit_vector(reflect(r_in.direction(), rec.normal)));
			return true;
		}
		scattered = ray(rec.p, refract(r_in.direction(), rec.normal, 1, effective_ri));
		return true;
	}
};



#endif