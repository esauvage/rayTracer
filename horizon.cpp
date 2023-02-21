#include "horizon.h"

Horizon::Horizon(float hauteur)
	:_hauteur(hauteur)
{
}

float Horizon::distance(const Vec3f& origin, const Vec3f& direction, float minDist) const
{
	(void)minDist;
	return (_hauteur-origin.z())/direction.z();
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
