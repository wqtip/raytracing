#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
private:
	point3 orig;
	vec3 dir;
public:
	ray(point3 origin, vec3 direction) :orig(origin), dir(direction) {};
	ray()
	{
		orig = vec3();
		dir = vec3();
	}
	point3 at(double t) const { return orig + t * dir; }
	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
};

#endif