#include "mesh.h"

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

bool Mesh::toucheTriangle(const Eigen::RowVector3i &triangle, const Rayon3f &r, double t_min, double t_max, HitRecord &rec) const
{
    std::array< Vec3f, 3 > _p {_points[triangle[0]], _points[triangle[1]], _points[triangle[2]]};
    auto _e1{_p[1] - _p[0]};
    auto _n((_p[1] - _p[0]).cross(_p[2] - _p[0]).normalized());
    auto _e2{_p[2] - _p[0]};
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
    const auto v = f * (r.direction().dot(q));

    if (v < 0 || (fabs(v) < 1e-5) || (u + v > 1.0))
        return false;

    rec.t = f * (_e2.dot(q));
    if (rec.t < t_min || fabs(rec.t-t_min) < 1e-5)
        return false;
    if (rec.t > t_max)
        return false;
    //On touche.
    //Trouvons les maximum 3 triangles qui partagent un côté avec celui-ci.
    vector <Eigen::Vector3i> communs;
    for (const auto & t1 : _iTriangles)
    {
        if (t1 == triangle)
        {
            continue;
        }
        auto nbCommuns = 0;
        for (auto i = 0; i < 3; ++i)
        {
            for (auto j = 0; j < 3; ++j)
            {
                if (t1[i] == triangle[j])
                {
                    ++nbCommuns;
                    break;
                }
            }
        }
        if (nbCommuns > 1)
        {
            communs.push_back(t1);
        }
    }
    vector<Vec3f> centres;
    for (const auto & i : communs)
    {
        centres.push_back(centre(i));
    }
    rec.p = r.at(rec.t);
    rec.setFaceNormal(r, _n);
    rec.pMaterial = material();

    return true;
}

Vec3f Mesh::centre(const Eigen::Vector3i &triangle) const
{
    Vec3f r;
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
