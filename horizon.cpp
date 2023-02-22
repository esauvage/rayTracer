#include "horizon.h"

Horizon::Horizon(float hauteur)
	:_hauteur(hauteur)
{
}

bool Horizon::touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const
{
	//TODO : vérifier.
	if (!r.direction().z())
		return false;
	rec.t = (_hauteur-r.origin().z())/r.direction().z();
	rec.p = r.at(rec.t);
	rec.normal = Vec3f(0, 0, 1);
	return true;
}

float Horizon::distance(const Rayon3f& r, float minDist) const
{
	(void)minDist;
	return (_hauteur-r.origin().z())/r.direction().z();
}

float Horizon::distanceMin(const Vec3f &origin) const
{
	(void) origin;
	return INFINITY;
}

Vec3f Horizon::normal(const Vec3f& p) const
{
	(void) p;
	return Vec3f{0, 0, 1};
}
