#include "horizon.h"

#include "utils.h"

Horizon::Horizon(float hauteur)
	:_hauteur(hauteur)
{
}

bool Horizon::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, ofstream& file) const
{
	if (!r.direction().z())
		return false;
	auto t = -(r.origin().z()-_hauteur)/r.direction().normalized().z();
	if (t < t_min || fabs(t - t_min) < 1e-5 || t > t_max)
		return false;
	rec.t = t;
	rec.p = r.pointAt(rec.t);
	rec.setFaceNormal(r, Vec3f(0, 0, 1));
	rec.pMaterial = material();
	return true;
}

json &Horizon::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	j["hauteur"] = _hauteur;
	return j;
}
