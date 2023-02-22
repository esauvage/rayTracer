#ifndef BOULE_H
#define BOULE_H

#include "shape.h"

class Boule : public Shape
{
public:
	Boule(const Vec3f& position, float rayon);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const;
	virtual float distance(const Rayon3f & r, float minDist = -1) const;
	virtual float distanceMax(const Rayon3f &r, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
private:
	Vec3f _pos;
	float _r;
};

#endif // BOULE_H
