#ifndef BOULE_H
#define BOULE_H

#include "shape.h"

class Boule : public Shape
{
public:
	Boule(const Vec3f& position, float rayon);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const;
	virtual json &jsonHelper(json& j) const;
private:
	Vec3f _pos;
	float _r;
};

#endif // BOULE_H
