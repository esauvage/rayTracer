#include "mesh.h"

#include <algorithm>
#include <iostream>

#include "utils.h"

using namespace nlohmann;
using namespace std;

Mesh::Mesh()
{

}

bool Mesh::touche(const Rayon3f &r, double t_min, double t_max, HitRecord &rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

//	for (const auto& object : _triangles)
//	{
//		if (object->touche(r, t_min, closest_so_far, temp_rec))
//		{
//			hit_anything = true;
//			closest_so_far = temp_rec.t;
//			rec = temp_rec;
//		}
//	}
	for (const auto& triangle : _iTriangles)
	{
        if (toucheTriangle(triangle, r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
	}

	return hit_anything;
}

bool Mesh::toucheTriangle(const Eigen::Vector3i &triangle, const Rayon3f &r, double t_min, double t_max, HitRecord &rec) const
{
	const float EPSILON = 1e-5;
	const Vec3f _p[3] {_points[triangle[0]], _points[triangle[1]], _points[triangle[2]]};
	const auto edge1{ _points[triangle[1]] - _p[0]};
	const auto edge2{ _points[triangle[2]] - _p[0]};
//	auto _n(edge1.cross(edge2).normalized());
	Vec3f h, s, q;
	float a,f,u,v;
	h = r.direction().cross(edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0/a;
	s = r.origin() - _p[0];
	u = f * s.dot(h);
	if ((u < 0.0) || (u > 1.0))
	{
		return false;
	}
	q = s.cross(edge1);
	v = f * r.direction().dot(q);
	if ((v < 0.0) || (u + v > 1.0))
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * edge2.dot(q);
	if (t < EPSILON || t < t_min || t > t_max) // ray intersection
	{
		return false;
	}
	rec.t = t;
	rec.p = r.pointAt(rec.t);
	//On touche.
	Vec3f rNorm;
	Vec3f baryPoint = barycentric(rec.p, _p[0], _p[1], _p[2]);
	for (int i = 0; i < 3; ++i)
	{
		rNorm += _normales.at(triangle[i]) * baryPoint[i];
	}
	//Cette ligne supprime le lissage des normales
//	rNorm = edge1.cross(edge2);
	rNorm.normalize();
	rec.setFaceNormal(r, rNorm);
    rec.pMaterial = material();

    return true;
}

Vec3f Mesh::centre(const Eigen::Vector3i &triangle) const
{
	Vec3f r(0., 0., 0.);
    for (const auto p : triangle)
    {
        r += _points.at(p);
    }
    return r/3.;
}

json &Mesh::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	for (const auto &o : _points)
	{
		j["points"] += json(o);
	}
	for (const auto &i : _iTriangles)
	{
		j["itriangles"] += json(i);
	}
	for (const auto &o : _triangles)
	{
		j["triangles"] += json(o);
	}
	return j;
}

void Mesh::add(shared_ptr<Shape> triangle)
{
	_triangles.push_back(triangle);
}

void Mesh::add(const Vec3f &point)
{
	_points.push_back(point);
}

void Mesh::add(const Eigen::Vector3i &triangle)
{
	_iTriangles.push_back(triangle);
}

//Calcule les normales en chaque point en faisant une moyenne par les angles.
//Source : https://stackoverflow.com/questions/45477806/general-method-for-calculating-smooth-vertex-normals-with-100-smoothness
void Mesh::update()
{
	_normales.assign(_points.size(), Vec3f(0, 0, 0));
	for(size_t i = 0; i < _iTriangles.size(); i++)
	{
		const auto v10 = _points[_iTriangles[i][1]] - _points[_iTriangles[i][0]];
		const auto v20 = _points[_iTriangles[i][2]] - _points[_iTriangles[i][0]];
		const auto v21 = _points[_iTriangles[i][2]] - _points[_iTriangles[i][1]];
		const auto v01 = _points[_iTriangles[i][0]] - _points[_iTriangles[i][1]];
		const auto v12 = _points[_iTriangles[i][1]] - _points[_iTriangles[i][2]];
		const auto v02 = _points[_iTriangles[i][0]] - _points[_iTriangles[i][2]];

		const auto fn = v10.cross(v20).normalized();
		auto a0 = acos(v10.dot(v20)/sqrt(v10.squaredNorm() * v20.squaredNorm()));    // p1 is the 'base' here
		auto a1 = acos(v21.dot(v01)/sqrt(v21.squaredNorm() * v01.squaredNorm()));    // p2 is the 'base' here
		auto a2 = acos(v02.dot(v12)/sqrt(v02.squaredNorm() * v12.squaredNorm()));    // p3 is the 'base' here
		if (a0 != a0)
			a0 = 0.;
		if (a1 != a1)
			a1 = 0.;
		if (a2 != a2)
			a2 = 0.;
		_normales[_iTriangles[i][0]] += fn * a0;
		_normales[_iTriangles[i][1]] += fn * a1;
		_normales[_iTriangles[i][2]] += fn * a2;
	}

	for (auto & n: _normales)
	{
		n.normalize();
	}
}
