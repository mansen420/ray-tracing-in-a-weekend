#ifndef MATERIAL
#define MATERIAL
#include "util.h"
#include "ray.h"
#include "geometry.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const hitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Color albedo;
public:
	Lambertian(const Color& a) { albedo = a; }
	virtual bool scatter(const Ray& r_in, const hitRecord& record, Color& attenuation, Ray& scatteredRay) const override
	{
		auto scatterDirection = record.normal + random_on_unit_sphere();
		if (scatterDirection.near_zero())
			scatterDirection = record.normal;
		scatteredRay = Ray(record.p, scatterDirection);
		attenuation = albedo;
		return true;
	}
};
class Metal : public Material
{
public:
	Color albedo;
	double fuzzFactor;
public:
	Metal(const Color& a, double f = 0) { albedo = a, fuzzFactor = f < 1 ? f : 1; }
	virtual bool scatter(const Ray& r_in, const hitRecord& record, Color& attenuation, Ray& scatteredRay) const override
	{
		Vec3 reflectedDirection = reflect(unitVector(r_in.direction), record.normal);
		scatteredRay = Ray(record.p, reflectedDirection + fuzzFactor*random_on_unit_sphere());
		attenuation = albedo;
		return Vec3::dot(scatteredRay.direction, record.normal) > 0;
	}
};
class Dielectric : public Material
{
public:
	double index_of_refraction;
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
public:
	Dielectric(double ir=0) { index_of_refraction = ir; }
	virtual bool scatter(const Ray& r_in, const hitRecord& record, Color& attenuation, Ray& scatteredRay) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);

		Vec3 unitDirection = unitVector(r_in.direction);
		double cos_theta = fmin(Vec3::dot(-unitDirection, record.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		double eta_ratio = record.frontFace ? 1.0 / index_of_refraction : index_of_refraction;
		bool must_reflect = eta_ratio * sin_theta > 1.0;
		Vec3 direction;
		if (must_reflect || reflectance(cos_theta, eta_ratio) > random_double())
			direction = reflect(unitDirection, record.normal);
		else
			direction = refract(unitDirection, record.normal, eta_ratio);

		scatteredRay = Ray(record.p, direction);
		return true;
	}
};
#endif // !MATERIAL

