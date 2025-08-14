#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <random>
#include <limits>
#include <numbers>
#include "vec3.h"
#include "ray.h"

using std::make_shared;
using std::shared_ptr;

static const double M_PI = 3.14159265358;
inline double random_double()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> distribution(0, 1);
	return distribution(gen);

}
inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}


inline vec3 cosine_sample_hemisphere(const vec3& normal)
{
    double r = sqrt(random_double(0,1));
    double theta = 2 * M_PI * random_double(0,1);
    double x = r * cos(theta);
    double y = r * sin(theta);
    double z = sqrt(1 - r * r);

    vec3 w = unit_vector(normal);
    vec3 a = (std::fabs(w.x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
    vec3 u = unit_vector(cross(a, w));
    vec3 v = cross(w, u);

    return x * u + y * v + z * w;
}

inline vec3 random_on_sphere()
{
	double x = 1.0, y = 1.0, z = 1.0;
	while (x * x + y * y + z * z < 1e-120 || x * x + y * y + z * z>1)
	{
		x = random_double(-1, 1);
		y = random_double(-1, 1);
		z = random_double(-1, 1);
	}
	vec3 ans(x, y, z);
	return unit_vector(ans);
}

inline std::vector<double> random_in_disk()
{
	double r = std::sqrt(random_double());
	double theta = 2 * M_PI * random_double();
	return { r * std::cos(theta),r * std::sin(theta) };
}

inline double dielectric_reflectance(double idx1, double idx2, double theta)
{
	double R0 = std::pow((idx1 - idx2) / (idx1 + idx2), 2);
	return R0 + (1 - R0) * std::pow(1 - std::cos(theta), 5);
}


#endif