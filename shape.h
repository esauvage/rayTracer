#ifndef SHAPE
#define SHAPE

#include "material.h"

class Shape
{
public:
	virtual ~Shape(){}
	virtual float distance(const Vec3f & origin, const Vec3f & direction, float minDist = -1) const = 0;
	virtual float distanceMax(const Vec3f & origin, const Vec3f & direction, float minDist = -1) const 
	{return distance(origin, direction, minDist);}
	virtual float distanceMin(const Vec3f & origin) const = 0;
	virtual Vec3f normal(const Vec3f & p) const = 0;
	Material *material {nullptr};
};
#endif // SHAPE
