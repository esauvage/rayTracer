#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"

using Rayon3f = Rayon<float, 3>;

struct hit_record {
	Vec3f p;
	Vec3f normal;
	float t;
	bool front_face;

	inline void setFaceNormal(const Rayon3f& r, const Vec3f& outward_normal) {
		front_face = r.direction().dot(outward_normal) < 0;
		normal = front_face ? outward_normal :-outward_normal;
	}
};

class Shape
{
public:
	virtual ~Shape(){}
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const = 0;
	virtual float distance(const Rayon3f & ray, float minDist = -1) const = 0;
	virtual float distanceMax(const Rayon3f & r, float minDist = -1) const
	{return distance(r, minDist);}
	virtual float distanceMin(const Vec3f & origin) const = 0;
	virtual Vec3f normal(const Vec3f & p) const = 0;
	Material *material {nullptr};
};
#endif // SHAPE
