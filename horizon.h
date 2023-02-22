#ifndef HORIZON_H
#define HORIZON_H

#include "shape.h"

class Horizon : public Shape
{
public:
	Horizon(float hauteur);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const;
	virtual float distance(const Rayon3f & r, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
private:
	float _hauteur;
};

#endif // HORIZON_H
