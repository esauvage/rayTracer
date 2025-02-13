#include "boule.h"

#include "utils.h"

#include "triangle.h"
#include <QDebug>

using namespace std;
using namespace Eigen;
using namespace nlohmann;

template <class T>
void toByteArray(QByteArray & a, T val)
{
    char *asChar = ( char* ) & val;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        a += asChar[i];
    }
    return;
}

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
    (void)time0;
    (void)time1;
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

void Boule::toIcosahedron(QList <Vec3f> &icosahedron)
{
    auto t = (1.0 + sqrt(5.0)) / 2.0;// Le nombre d'or !
    auto l = _r / sqrt(1 + t * t);//Rayon de la sphère
    //Un icosaèdre, c'est 4 rectangles orthogonaux. Les voici :
    icosahedron << Vec3f(-1,  t,  0)*l;
    icosahedron << Vec3f( 1,  t,  0)*l;
    icosahedron << Vec3f(-1, -t,  0)*l;
    icosahedron << Vec3f( 1, -t,  0)*l;

    icosahedron << Vec3f( 0, -1,  t)*l;
    icosahedron << Vec3f( 0,  1,  t)*l;
    icosahedron << Vec3f( 0, -1, -t)*l;
    icosahedron << Vec3f( 0,  1, -t)*l;

    icosahedron << Vec3f( t,  0, -1)*l;
    icosahedron << Vec3f( t,  0,  1)*l;
    icosahedron << Vec3f(-t,  0, -1)*l;
    icosahedron << Vec3f(-t,  0,  1)*l;
}

int Boule::serialize(QByteArray &dest)
{
    QList <Vec3f> icosahedron;
    toIcosahedron(icosahedron);
    //Maintenant, il faut définir les faces à partir des points de l'icosahèdre
    QList <Triangle> _triangles;
    // 5 faces autour du point 0
    _triangles << Triangle(icosahedron[0], icosahedron[11], icosahedron[5]);
    _triangles << Triangle(icosahedron[0], icosahedron[5], icosahedron[1]);
    _triangles << Triangle(icosahedron[0], icosahedron[1], icosahedron[7]);
    _triangles << Triangle(icosahedron[0], icosahedron[7], icosahedron[10]);
    _triangles << Triangle(icosahedron[0], icosahedron[10], icosahedron[11]);

    // 5 adjacent faces
    _triangles << Triangle(icosahedron[1], icosahedron[5], icosahedron[9]);
    _triangles << Triangle(icosahedron[5], icosahedron[11], icosahedron[4]);
    _triangles << Triangle(icosahedron[11], icosahedron[10], icosahedron[2]);
    _triangles << Triangle(icosahedron[10], icosahedron[7], icosahedron[6]);
    _triangles << Triangle(icosahedron[7], icosahedron[1], icosahedron[8]);

    // 5 faces around point 3
    _triangles << Triangle(icosahedron[3], icosahedron[9], icosahedron[4]);
    _triangles << Triangle(icosahedron[3], icosahedron[4], icosahedron[2]);
    _triangles << Triangle(icosahedron[3], icosahedron[2], icosahedron[6]);
    _triangles << Triangle(icosahedron[3], icosahedron[6], icosahedron[8]);
    _triangles << Triangle(icosahedron[3], icosahedron[8], icosahedron[9]);

    // 5 adjacent faces
    _triangles << Triangle(icosahedron[4], icosahedron[9], icosahedron[5]);
    _triangles << Triangle(icosahedron[2], icosahedron[4], icosahedron[11]);
    _triangles << Triangle(icosahedron[6], icosahedron[2], icosahedron[10]);
    _triangles << Triangle(icosahedron[8], icosahedron[6], icosahedron[7]);
    _triangles << Triangle(icosahedron[9], icosahedron[8], icosahedron[1]);

    int nbPoints = 0;
    for (auto t : _triangles)
    {
        nbPoints += t.serialize(dest);
    }
    return nbPoints;
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);

//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);

//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);

//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);

//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 1.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, 0.f);
//    toByteArray<float>(dest, -1.f);
//    return 12;
}

Vec2f Boule::UV(const Vec3f& p) const
{
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
    Eigen::Quaternion<double> rotation(0.2705981, 0.2705981, -0.6532815, 0.6532815);
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
