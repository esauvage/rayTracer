#ifndef BOULE_H
#define BOULE_H

#include "shape.h"

class Boule : public Shape
{
public:
	Boule(const Vec3f& position, float rayon);
	virtual float distance(const Vec3f & origin, const Vec3f & direction, float minDist = -1) const;
	virtual float distanceMax(const Vec3f & origin, const Vec3f & direction, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
private:
	Vec3f _pos;
	float _r;
};

#endif // BOULE_H
