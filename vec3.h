#ifndef VEC3_H
#define VEC3_H

#include <cmath>
class vec3 {
public:
	double vec[3];
	vec3(double x, double y, double z)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}
	vec3()
	{
		vec[0] = 0.0;
		vec[1] = 0.0;
		vec[2] = 0.0;
	}
	double x() { return vec[0]; }
	double y() { return vec[1]; }
	double z() { return vec[2]; }

	double& operator[](int idx) { return vec[idx]; }
	double operator[](int idx) const { return vec[idx]; }
	vec3 operator-() const { return vec3(-vec[0], -vec[1], -vec[2]); }
};

typedef vec3 color;
typedef vec3 point3;
inline std::ostream& operator<<(std::ostream& os,const vec3& a)
{
	os << a[0] << " " << a[1] << " " << a[2] << "\n";
	return os;
}
inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]); }
inline vec3 operator-(const vec3& a, const vec3& b) { return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]); }
inline vec3 operator*(double d, const vec3& a) { return vec3(d * a[0], d * a[1], d * a[2]); }
inline vec3 operator*(const vec3& a, double d) { return d * a; }
inline vec3 hadamard_product(const vec3& a, const vec3& b) { return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]); }
inline vec3 operator/(const vec3& a, double d) { return (1 / d) * a; }
inline double dot(const vec3& a, const vec3& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0]); }
inline double length(const vec3& a) { return std::sqrt(dot(a, a)); }
inline vec3 unit_vector(const vec3& a) { return a / length(a); }
inline vec3 reflect(const vec3& a, const vec3& b) { return a - 2 * dot(a, b) * b; }
inline vec3 refract(vec3 v_in, vec3& normal, double refraction_idx_in, double refraction_idx_out)
{
	v_in = unit_vector(v_in);
	normal = unit_vector(normal);
	double theta1 = std::acos(dot(-v_in, normal));
	double theta2 = std::asin((refraction_idx_in / refraction_idx_out) * std::sin(theta1));
	vec3 proj_in_onto_normal = dot(v_in, normal) * normal;
	vec3 orthogonal_to_normal_component = unit_vector(v_in - proj_in_onto_normal);
	vec3 parallel_to_normal_component = (-normal) / std::tan(theta2);
	return unit_vector(orthogonal_to_normal_component + parallel_to_normal_component);
}

#endif