#include "boule.h"

Boule::Boule(const Vec3f& position, float rayon)
	:_pos{position}, _r{rayon}
{
}

float Boule::distance(const Vec3f& origin, const Vec3f& direction, float minDist) const
{
	Vec3f p(origin - _pos);
	if (minDist > 0)
	{
		if (p.x() > minDist + _r)
			return -1;
		if (p.y() > minDist + _r)
			return -1;
		if (p.z() > minDist + _r)
			return -1;
	}
	const auto b = (p.dot(direction));
	const auto c = (p.dot(p) - _r * _r);
	const auto q = b * b - c;
	if (q >= 0)
	{
		const auto sqrtQ = sqrt(q);
		const float r1 = -b-sqrtQ;
		const float r2 = -b+sqrtQ;
		return (r2 > 0 && r2 < r1) ? r2 : r1;
	}
	return -1;
}

float Boule::distanceMax(const Vec3f & origin, const Vec3f & direction, float minDist) const
{
	Vec3f p(origin - _pos);
	if (minDist > 0)
	{
		if (p.x() > minDist + _r)
			return -1;
		if (p.y() > minDist + _r)
			return -1;
		if (p.z() > minDist + _r)
			return -1;
	}
	const auto b = (p.dot(direction));
	const auto c = (p.dot(p) - _r * _r);
	const auto q = b * b - c;
	if (q >= 0)
	{
		const auto sqrtQ = sqrt(q);
		const float r1 = -b-sqrtQ;
		const float r2 = -b+sqrtQ;
		return (r2 > 0 && r2 < r1) ? r1 : r2;
	}
	return -1;
}

float Boule::distanceMin(const Vec3f &origin) const
{
	return origin.dot(_pos) - _r * _r;
}

Vec3f Boule::normal(const Vec3f& p) const
{
	return (p - _pos).normalized();
}
