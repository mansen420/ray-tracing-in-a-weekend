#ifndef HITTABLE_LIST
#define HITTABLE_LIST
#include "geometry.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
	virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& record) const override;
	HittableList(){}
	HittableList(std::shared_ptr<Hittable> object)
	{
		add(object);
	}
	void add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}
	void clear()
	{
		objects.clear();
	}
	std::vector<std::shared_ptr<Hittable>> objects;
};
bool HittableList::hit(const Ray& r, double tMin, double tMax, hitRecord& record) const
{
	hitRecord tempRecord;
	bool hitAnything{ false };
	double closestSoFar = tMax;
	for (const auto& object : objects)
	{
		if (object->hit(r, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}
#endif // !HITTABLE_LIST

