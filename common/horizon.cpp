#include "horizon.h"

#include "utils.h"
#include <cmath>
using namespace std;
using namespace nlohmann;

Horizon::Horizon(float hauteur)
	:_hauteur(hauteur)
{
}

bool Horizon::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
{
	if (!r.direction().z())
		return false;
	auto t = -(r.origin().z()-_hauteur)/r.direction().normalized().z();
	if (t < t_min || fabs(t - t_min) < 1e-5 || t > t_max || t != t)
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

bool Horizon::boundingBox(double time0, double time1, AABB &outputBox) const
{
    (time0);
    Vec3f min(-INFINITY, -INFINITY, -INFINITY);
    Vec3f max(INFINITY, INFINITY, _hauteur);
    outputBox = AABB(min, max);
    return true;
}
