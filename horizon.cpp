#include "horizon.h"

#include "utils.h"

Horizon::Horizon(float hauteur)
	:_hauteur(hauteur)
{
}

bool Horizon::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const
{
	//TODO : vérifier.
	if (!r.direction().z())
		return false;
	rec.t = (_hauteur-r.origin().z())/r.direction().normalized().z();
	if (rec.t < t_min || rec.t > t_max)
		return false;
	rec.p = r.at(rec.t);
	rec.setFaceNormal(r, Vec3f(0, 0, 1));
	rec.pMaterial = material();
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
