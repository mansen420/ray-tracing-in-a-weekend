#ifndef CAMERA
#define CAMERA
#include "vec3.h"
#include "ray.h"

class Camera
{
public:
	Camera (){}
	Point lookfrom = Point(-2, 2, 1);  // Point camera is looking from
	Point lookat = Point(0, 0, -1);   // Point camera is looking at
	Vec3   vup = Vec3(0, 1, 0);     // Camera-relative "up" direction

	double fov = 20;
	double theta = degrees_to_radians(fov);
	double h = tan(theta / 2);

	const double aspectRatio = 16.0 / 9.0;
	double focalLength = (lookfrom - lookat).magnitude();
	double viewPortHeight = focalLength * 2 * h;
	double viewPortWidth = viewPortHeight * aspectRatio;

	Vec3 w = unitVector(lookfrom - lookat);
	Vec3 u = unitVector(Vec3::cross(vup, w));
	Vec3 v = Vec3::cross(w, u);

	Vec3 horizontalOffset = u * viewPortWidth;
	Vec3 verticalOffset = -v * viewPortHeight;
	Point origin = lookfrom;
	Point upperLeftCorner = origin - (0.5 * horizontalOffset) - (0.5 * verticalOffset) - (focalLength * w);


	Ray viewportTraversal(double u, double v)
	{
		return Ray(origin, upperLeftCorner + u * horizontalOffset + v * verticalOffset - origin);
	}
};
#endif // !CAMERA

