#include "triangle.h"

Triangle::Triangle(const array <Vec3f, 3> &points)
	:_p(points), _n((_p[0] - _p[1]).cross(_p[2] - _p[1]).normalized()), _e1{_p[1] - _p[0]}, _e2{_p[2] - _p[0]}
{
}

bool Triangle::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const
{
	const auto h = r.direction().cross(_e2);
	const auto a = _e1.dot(h);

	if (fabs(a) < 0.00001)
		return false;

	const auto f = 1. / a;
	const auto s {r.origin() - _p[0]};

	const auto u = s.dot(h) * f;

	if ((u < 0.0) || (u > 1.0))
		return false;

	const auto q = s.cross(_e1);
	const auto v = f * (r.direction().dot(q));

	if ((v < 0.0) || (u + v > 1.0))
		return false;

	rec.t = f * (_e2.dot(q));
	rec.normal = _n;
	rec.p = r.at(rec.t);

	return true;
}

float Triangle::distance(const Rayon3f & r, float minDist) const
{
	(void)minDist;
	const auto h = r.direction().cross(_e2);
	const auto a = _e1.dot(h);

	if (a > -0.00001 && a < 0.00001)
		return -1;

	const auto f = 1. / a;
	const auto s = r.origin() - _p[0];

	const auto u = (s.dot(h)) * f;

	if ((u < 0.0) || (u > 1.0))
		return -1;

	const auto q = s.cross(_e1);
	const auto v = f * (r.direction().dot(q));

	if ((v < 0.0) || (u + v > 1.0))
		return -1;

	return f * (_e2.dot(q));
}

float Triangle::distanceMin(const Vec3f &origin) const
{
	auto d = origin.dot(_p[0]);
	d = std::min(d, origin.dot(_p[1]));
	d = std::min(d, origin.dot(_p[2]));
	return d;
}

Vec3f Triangle::normal(const Vec3f & p) const
{
	(void)p;
	return _n;
}
