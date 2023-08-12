#include<iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "geometry.h"
#include "HittableList.h"
#include "util.h"
#include "camera.h"
#include "material.h"
using std::cout;
void init(int height, int width);
Color background(const Ray& r, const Hittable& objects, int recursionDepth);
const double aspectRatio = 16.0 / 9.0;
const int imageHeight = 400;
const int imageWidth = static_cast<int>(imageHeight * aspectRatio);
const int samplesPerPixel = 25;
const int recursioDepth = 10;
std::ofstream imageStream("image.PPM");
int main()
{
	Camera cam;
	//world objects
	HittableList objects;
	/* auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left = make_shared<Dielectric>(1.5);
	auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

	objects.add(make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
	objects.add(make_shared<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
	objects.add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
	objects.add(make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right)); */
	auto R = cos(pi / 4);

	auto material_left = make_shared<Lambertian>(Color(0, 0, 1));
	auto material_right = make_shared<Lambertian>(Color(1, 0, 0));

	objects.add(make_shared<Sphere>(Point(-R, 0, -1), R, material_left));
	objects.add(make_shared<Sphere>(Point(R, 0, -1), R, material_right));

	init(imageHeight, imageWidth);
	for (int i = 0; i < imageHeight; i++)
	{
	std::cerr << "\rLines remaining : " << imageHeight-i << "\n" << std::flush;
		for (int j = 0; j < imageWidth; j++)
		{
			Color baseColor(0, 0, 0);
			for (int s = 0; s < samplesPerPixel; s++)
			{
				double u = (double(j)+random_double()) / (imageWidth - 1);
				double v = (double(i)+random_double()) / (imageHeight - 1);
				Ray viewportTraversal = cam.viewportTraversal(u, v);
				baseColor = baseColor + background(viewportTraversal, objects, recursioDepth);
			}
			writeColor(imageStream, baseColor, samplesPerPixel);
		}
	}
	std::cerr << "\nDone\n";

	imageStream.close();
}
void init(int height, int width)
{
	imageStream << "P3\n" << width << " " << height << "\n255" << std::endl;
}
Color background(const Ray& r, const Hittable& objects, int recursionDepth)
{
	if (recursionDepth <= 0)
		return Color(0,0,0);
	hitRecord record;
	if (objects.hit(r, 0.0001, infinity, record))
	{
		Ray scatteredRay;
		Color attenuation;
		if (record.matPtr->scatter(r, record, attenuation, scatteredRay))
		{
			return attenuation * background(scatteredRay, objects, recursionDepth - 1);
		}
	}
	double t;
	auto unitDirection = unitVector(r.direction);
	t = 0.5 * unitDirection.y + 0.5;
	return (1.0 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1.0);
}