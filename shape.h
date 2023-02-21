#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"

using Rayon3f = Rayon<float, 3>;

struct hit_record {
	Vec3f p;
	Vec3f normal;
	float t;
};

class Shape
{
public:
	virtual ~Shape(){}
	virtual float distance(const Rayon3f & ray, float minDist = -1) const = 0;
	virtual float distanceMax(const Rayon3f & r, float minDist = -1) const
	{return distance(r, minDist);}
	virtual float distanceMin(const Vec3f & origin) const = 0;
	virtual Vec3f normal(const Vec3f & p) const = 0;
	Material *material {nullptr};
};
#endif // SHAPE
