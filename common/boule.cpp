#include "boule.h"

#include "utils.h"

using namespace std;
using namespace Eigen;
using namespace nlohmann;

Boule::Boule(const Vec3f& position, float rayon)
	:_pos{position}, _r{rayon}
{
}

bool Boule::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
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
    rec.setTex(UV(rec.p));
	rec.pMaterial = material();

	return true;
}

bool Boule::boundingBox(double time0, double time1, AABB &outputBox) const
{
    outputBox = AABB(
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

void Boule::serialize(QByteArray &dest)
{

}

Vec2f Boule::UV(const Vec3f& p) const
{
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    Eigen::Quaternion<float> rotation(0.2705981, 0.2705981, -0.6532815, 0.6532815);
    Vec3f pLocal = rotation * p;

    float theta = acos(-pLocal.y());
    if (theta != theta)
    {
        theta = 0;
    }
    float phi = atan2(-pLocal.z(), pLocal.x()) + M_PI;

    const float u = phi / (2*M_PI);
	const float v = theta / M_PI;
	return Vec2f(u, v);
}
