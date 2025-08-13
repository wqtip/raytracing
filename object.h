#ifndef OBJECT_H
#define OBJECT_H

class material;

class hit_record {
public:
	ray r;
	double t;
	point3 p;
	vec3 normal;
	bool front_face;
	std::shared_ptr<material> mat;

	~hit_record() = default;
	hit_record()
	{
		r = ray();
		t = 0;
		p = vec3();
		normal = vec3();
		front_face = false;
	};

	void set_surface_normal()
	{
		front_face = dot(r.direction(), normal) < 0;
		normal = (dot(r.direction(), normal) < 0) ? normal : -normal;
	}
};

class object {
public:
	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	~object() = default;
};

#endif
