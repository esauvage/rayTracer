#include "triangle.h"

#include "utils.h"

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

Triangle::Triangle(const std::array <Vec3f, 3> &points)
	:_p(points), _n((_p[1] - _p[0]).cross(_p[2] - _p[0]).normalized()), _e1{_p[1] - _p[0]}, _e2{_p[2] - _p[0]}
{
	_tex[0] = Vec2f(0,0);
	_tex[1] = Vec2f(1,0);
	_tex[2] = Vec2f(1,1);
}

Triangle::Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c)
    :Triangle(std::array<Vec3f, 3>{a, b, c})
{

}

bool Triangle::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
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
	Vec2f tex(0, 0);
	tex = _tex[0] * u + _tex[1] * v + _tex[2] * (1 - u - v);
	rec.setTex(tex);
	return true;
}

json &Triangle::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	j["points"] = _p;
	return j;
}

void Triangle::setTex(const std::array<Vec2f, 3> &newTex)
{
	_tex = newTex;
}

void Triangle::setTex(const Vec2f &a, const Vec2f &b, const Vec2f &c)
{
    setTex(std::array<Vec2f, 3>{a, b, c});
}

int Triangle::serialize(QByteArray &dest)
{
    for (auto i = 0; i < 3; ++i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            toByteArray<float>(dest, _p[i][j]);
        }
        toByteArray<float>(dest, _tex[i][0]);
        toByteArray<float>(dest, _tex[i][1]);
        for (auto j = 0; j < 3; ++j)
        {
            toByteArray<float>(dest, _n[j]);
        }
    }
    for (auto i = 2; i >= 0; --i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            toByteArray<float>(dest, _p[i][j]);
        }
        toByteArray<float>(dest, _tex[i][0]);
        toByteArray<float>(dest, _tex[i][1]);
        for (auto j = 0; j < 3; ++j)
        {
            toByteArray<float>(dest, -_n[j]);
        }
    }
    return 6;
}

bool Triangle::boundingBox(double time0, double time1, AABB &outputBox) const
{
	Vec3f min = _p[0];
	Vec3f max = _p[0];
	for (const auto &p : _p)
	{
		min = p.array().min(min.array());
		max = p.array().max(max.array());
	}
    outputBox = AABB(min, max);
	return true;
}
