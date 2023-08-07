#ifndef RAY
#define RAY
#include "vec3.h"
class Ray
{
public:
	Point origin;
	Vec3 direction;
	Ray()
	{
		origin = Vec3();
		direction = Vec3();
	}
	Ray(Point origin, Vec3 direction)
	{
		this->origin = origin;
		this->direction = direction;
	}
	Point at(double t) const
	{
		return origin+t*direction;
	}
};
#endif // !RAY

