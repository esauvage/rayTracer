#include "boule.h"

#include <iostream>

#include "utils.h"

using namespace std;

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
	rec.p = r.at(rec.t);
	const Vec3f outwardNormal = (rec.p - _pos) / _r;
	rec.setFaceNormal(r, outwardNormal);
	rec.pMaterial = material();

	if (root != root)
	{
		cout << "Rayon : " << r << endl;
		cout << a << " discriminant " << discriminant << " half b " << half_b << " oc " << oc << endl;
		cout << " position : " << _pos << endl;
	}
	return true;
}


float Boule::distance(const Rayon3f& r, float minDist) const
{
	Vec3f p(r.origin() - _pos);
	if (minDist > 0)
	{
		if (p.x() > minDist + _r)
			return -1;
		if (p.y() > minDist + _r)
			return -1;
		if (p.z() > minDist + _r)
			return -1;
	}
	const auto b = (p.dot(r.direction()));
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

float Boule::distanceMax(const Rayon3f & r, float minDist) const
{
	Vec3f p(r.origin() - _pos);
	if (minDist > 0)
	{
		if (p.x() > minDist + _r)
			return -1;
		if (p.y() > minDist + _r)
			return -1;
		if (p.z() > minDist + _r)
			return -1;
	}
	const auto b = (p.dot(r.direction()));
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
