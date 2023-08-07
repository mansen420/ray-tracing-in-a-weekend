#ifndef GEOMETRY
#define GEOMETRY
#include "vec3.h"
#include "ray.h"
class Material;
struct hitRecord
{
	shared_ptr<Material> matPtr;
	Point p;
	Vec3 normal;
	double t;
	bool frontFace;
	inline void setFaceNormal(const Ray& r, Vec3 outwardNormal)
	{
		frontFace = Vec3::dot(r.direction, outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};
class Hittable
{
public:
	virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& record) const = 0;
};
class Sphere : public Hittable
{
public:
	double radius;
	Point center;
	shared_ptr<Material> matPtr;
	Sphere(Point c, double r, shared_ptr<Material> m)
	{
		center = c, radius = r, matPtr = m;
	}
	Sphere(Point c, double r) 
	{
		center = c, radius = r, matPtr = nullptr;
	}
	Sphere()
	{
		center = Point();
		radius = 0.0;
	}
	bool hit(const Ray& r, double tMin, double tMax, hitRecord& record) const override
	{
		auto oc = r.origin - center;
		auto a = Vec3::dot(r.direction, r.direction);
		auto half_b = Vec3::dot(oc, r.direction);
		auto c = Vec3::dot(oc, oc) - radius * radius;
		auto discriminant = (half_b * half_b) - (a * c);
		auto root = (-half_b - sqrt(discriminant)) / (a);
		if (discriminant < 0)
		{
			return false;
		}
		if (root > tMax || root < tMin)
		{
			root = (-half_b + sqrt(discriminant)) / a;
			if (root > tMax || root < tMin)
				return false;
		}
		record.t = root;
		record.p = r.at(record.t);
		Vec3 outwardNormal = (record.p - center) / radius;
		record.setFaceNormal(r, outwardNormal);
		record.matPtr = this->matPtr;
		return true;
	}
};

#endif // !GEOMETRY

