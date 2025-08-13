#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "material.h"

class sphere : public object{
private:
	point3 center;
	double radius;
	std::shared_ptr<material> mat;
public:
	sphere(point3 center, double radius, std::shared_ptr<material> mat) :center(center), radius(radius), mat(mat) {};

	bool hit(ray& r, double t_min, double t_max, hit_record& rec) const override {
		double a = dot(r.direction(), r.direction());
		double b = 2 * (dot(r.origin(), r.direction()) - dot(center, r.direction()));
		double c = dot(center, center) + dot(r.origin(), r.origin()) - 2 * dot(center, r.origin()) - radius * radius;
		double discriminant = b * b - 4 * a * c;
		
		if (discriminant < 0) return false;

		double root = (-b - std::sqrt(discriminant)) / (2 * a);
		if (root<t_min || root>t_max)
		{
			root = (-b + std::sqrt(discriminant)) / (2 * a);
			if (root<t_min || root>t_max) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = unit_vector(rec.p - center);
		rec.r = r;
		rec.set_surface_normal();
		rec.mat = mat;
		return true;
	}
};

#endif