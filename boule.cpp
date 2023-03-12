#include "boule.h"

#include "utils.h"

using namespace std;
using namespace Eigen;

Boule::Boule(const Vec3f& position, float rayon)
	:_pos{position}, _r{rayon}
{
}

bool Boule::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const
{
	const Vec3f oc = r.origin() - _pos;
	const auto a = r.direction().squaredNorm();
	const auto half_b = oc.dot(r.direction());
	const auto c = oc.squaredNorm() - _r*_r;

	const auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0)
	{
		return false;
	}
	const auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || fabs(root - t_min) < 1e-5 || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < t_min || fabs(root - t_min) < 1e-5 || t_max < root)
			return false;
	}
	rec.t = root;
	rec.p = r.pointAt(rec.t);
	const Vec3f outwardNormal = (rec.p - _pos) / _r;
	rec.setFaceNormal(r, outwardNormal);
	rec.pMaterial = material();

	return true;
}

bool Boule::boundingBox(double time0, double time1, AlignedBox3f &outputBox) const
{
	outputBox = AlignedBox3f(
		_pos - Vec3f(_r, _r, _r),
		_pos + Vec3f(_r, _r, _r));
	return true;
}

json &Boule::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	j["position"] = _pos;
	j["rayon"] = _r;
	return j;
}
