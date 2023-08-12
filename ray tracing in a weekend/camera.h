#ifndef CAMERA
#define CAMERA
#include "vec3.h"
#include "ray.h"

class Camera
{
public:
	Camera (){}
	double fov = 90;
	double theta = degrees_to_radians(fov);
	double h = tan(theta / 2);

	const double aspectRatio = 16.0 / 9.0;
	double focalLength = 1.0;
	double viewPortHeight = focalLength * 2 * h;
	double viewPortWidth = viewPortHeight * aspectRatio;

	Vec3 horizontalOffset = Vec3(viewPortWidth, 0, 0);
	Vec3 verticalOffset = Vec3(0, -viewPortHeight, 0);
	Point origin = Point(0, 0, 0);
	Point upperLeftCorner = origin - (0.5 * horizontalOffset) - (0.5 * verticalOffset) - Vec3(0, 0, focalLength);


	Ray viewportTraversal(double u, double v)
	{
		return Ray(origin, upperLeftCorner + u * horizontalOffset + v * verticalOffset - origin);
	}
};
#endif // !CAMERA

