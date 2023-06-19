#include "triangle.h"

#include "utils.h"

using namespace std;

Triangle::Triangle(const array <Vec3f, 3> &points)
	:_p(points), _n((_p[1] - _p[0]).cross(_p[2] - _p[0]).normalized()), _e1{_p[1] - _p[0]}, _e2{_p[2] - _p[0]}
{
}

bool Triangle::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, ofstream& file) const
{
	const Vec3f h = r.direction().cross(_e2);
	const auto a = _e1.dot(h);

	if (fabs(a) < 1e-5)
		return false;

	const auto f = 1. / a;
	const Vec3f s {r.origin() - _p[0]};

	const auto u = s.dot(h) * f;

	if (u < 0 || (fabs(u) < 1e-5) || (u > 1.0))
		return false;

	const Vec3f q = s.cross(_e1);
	if (q.hasNaN())
	{
		return false;
	}
	const auto v = f * (r.direction().dot(q));

	if (v < 0 || (fabs(v) < 1e-5) || (u + v > 1.0))
		return false;

	auto t = f * (_e2.dot(q));
	if (t < t_min || fabs(t-t_min) < 1e-5)
		return false;
	if (t > t_max)
		return false;
	rec.t = t;
	rec.p = r.pointAt(rec.t);
	rec.setFaceNormal(r, _n);
	rec.pMaterial = material();

	return true;
}

json &Triangle::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	j["points"] = _p;
	return j;
}
