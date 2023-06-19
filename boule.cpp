#include "boule.h"

#include "utils.h"

using namespace std;
using namespace Eigen;

Boule::Boule(const Vec3f& position, float rayon)
	:_pos{position}, _r{rayon}
{
}

bool Boule::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, ofstream& file) const
{
	const Vec3f oc = r.origin() - _pos;
	const float a = r.direction().squaredNorm();
	const float half_b = oc.dot(r.direction());
	const float c = oc.squaredNorm() - _r*_r;

	const float discriminant = half_b*half_b - a*c;
	if (discriminant < 0)
	{
		return false;
	}
	const float sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	float root = (-half_b - sqrtd) / a;
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
	rec.setTex(getUV(rec.p));
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

Vec2f Boule::getUV(const Vec3f& p) const
{
	Vec3f pUnit = p/_r;
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
	//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

	float theta = acos(-p.y());
	float phi = atan2(-p.z(), p.x()) + M_PI;

	const float u = phi / (2*M_PI);
	const float v = theta / M_PI;
	return Vec2f(u, v);
}
