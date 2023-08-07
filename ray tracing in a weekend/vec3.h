#ifndef VEC3
#define VEC3
#include <cmath>
#include <iostream>
#include "util.h"
class Vec3
{
private :
	double e[3];
public:
	Vec3(const Vec3& u)
	{
		x = r = e[0] = u.x, y = g = e[1] = u.y, z = b = e[2] = u.z;
	}
	Vec3(double e0 = 0, double e1 = 0, double e2 = 0) { x = r = e[0] = e0, y = g = e[1] = e1, z = b = e[2] = e2; }
	double x, y, z;
	double r, g, b;
	//operator overloading
	Vec3& operator+=(const Vec3& u)
	{
		e[0] += u.x;
		e[1] += u.y;
		e[2] += u.z;
		return *this;
	}
	Vec3& operator+=(const double t)
	{
		e[0], e[1], e[2] += t;
		return *this;
	}
	Vec3& operator*=(const double t)
	{
		e[0], e[1], e[2] *= t;
		return *this;
	}
	//calculation functions
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	double magnitude () const
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	double dot(const Vec3& v) const
	{
		return v.x * x + v.y * y + v.z * z;
	}
	static double dot(const Vec3& v, const  Vec3& u)
	{
		return v.x * u.x + v.y * u.y + v.z * u.z;
	}
	static Vec3 cross(const Vec3& u, const Vec3& v)
	{
		return Vec3(u.y * v.z - u.z * v.y,
					u.z * v.x - u.x * v.z,
					u.x * v.y - v.x * u.y);
	}
	inline static Vec3 random() {
		return Vec3(random_double(), random_double(), random_double());
	}

	inline static Vec3 random(double min, double max) {
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};
using Color = Vec3;
using Point = Vec3;
//operator overloading 
//functions that dont involve direct changes to 
//the member variables of Vec3 will be defined here
//to reduce clutter in Vec3, function definitions that only call other functions will also be included here
Vec3 operator-(const Vec3& u)
{
	return Vec3(-u.x, -u.y, -u.z);
}
Vec3& operator-=(Vec3& v, Vec3& u)
{
	return v += -u;
}
Vec3& operator/=(Vec3& v,const double t)
{
	return v *= 1 / t;
}
Vec3& operator-=(Vec3& v, const double t)
{
	return v += -t;
}


Vec3 operator+(const Vec3& v, const Vec3& u)
{
	return Vec3(v.x + u.x, v.y + u.y, v.z + u.z);
}
Vec3 operator-(const Vec3& v, const Vec3& u)
{
	return v + -u;
}


Vec3 operator+(const Vec3& v, const double t)
{
	return Vec3(v.x + t, v.y + t, v.z + t);
}
Vec3 operator-(const Vec3& v, const double t)
{
	return v + -t;
}
Vec3 operator*(const Vec3& v, const double t)
{
	return Vec3(v.x * t, v.y * t, v.z * t);
}
Vec3 operator/(const Vec3& v, const double t)
{
	return v * (1 / t);
}


Vec3 operator+(const double t, const Vec3& v)
{
	return v + t;
}
Vec3 operator-(const double t, const Vec3& v)
{
	return t + -v;
}
Vec3 operator*(const double t, const Vec3& v)
{
	return v * t;
}

Vec3 operator*(const Vec3& v, const Vec3 u)
{
	return Vec3(v.x * u.x, v.y * u.y, v.z * u.z);
}

Vec3 unitVector(const Vec3& u)
{
	return u / u.magnitude();
}


std::ostream& writeColor(std::ostream& stream, Color normalizedPixelRGB, int samplesPerPixel)
{
	auto r = sqrt(normalizedPixelRGB.r/samplesPerPixel);
	auto g = sqrt(normalizedPixelRGB.g/samplesPerPixel);
	auto b = sqrt(normalizedPixelRGB.b/samplesPerPixel);

	stream << static_cast<int>(clamp(r, 0.0, 0.999) * 256) << " "
		<< static_cast<int>(clamp(g, 0.0, 0.999) * 256) << " "
		<< static_cast<int>(clamp(b, 0.0, 0.999) * 256) << "\n";
	return stream;
}

Vec3 random_in_unit_sphere()
{
	while (true) {
		auto p = Vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
Vec3 random_on_unit_sphere()
{
	return unitVector(random_in_unit_sphere());
}
Vec3 random_in_hemisphere(const Vec3& normal)
{
	Vec3 randomInUnitSphere = random_in_unit_sphere();
	if (randomInUnitSphere.dot(normal) <= 0)
		return -randomInUnitSphere;
	else
		return randomInUnitSphere;
}
Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * Vec3::dot(v, n) * n;
}
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(Vec3::dot(-uv, n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}
#endif // !VEC3
