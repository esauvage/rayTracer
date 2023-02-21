#include "triangle.h"

Triangle::Triangle(const array <Vec3f, 3> &points)
	:_p(points), _n((_p[0] - _p[1]).cross(_p[2] - _p[1]).normalized()), _e1{_p[1] - _p[0]}, _e2{_p[2] - _p[0]}
{
}

float Triangle::distance(const Vec3f & origin, const Vec3f & direction, float minDist) const
{
	const Vec3f h {direction ^ _e2};
	const float a = _e1.dot(h);

	if (a > -0.00001 && a < 0.00001)
		return -1;

	const float f = 1. / a;
	const Vec3f s {origin - _p[0]};

	const float u = (s.dot(h)) * f;

	if (u < 0.0 || u > 1.0)
		return -1;

	const Vec3f q {s ^ _e1};
	const float v = f * (direction.dot(q));

	if (v < 0.0 || u + v > 1.0)
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
