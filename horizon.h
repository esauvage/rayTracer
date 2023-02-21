#ifndef HORIZON_H
#define HORIZON_H

#include "shape.h"

class Horizon : public Shape
{
public:
	Horizon(float hauteur);
	virtual float distance(const Vec3f & origin, const Vec3f & direction, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
private:
	float _hauteur;
};

#endif // HORIZON_H
