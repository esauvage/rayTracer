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

	for (const auto& object : _triangles)
	{
		if (object->touche(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	for (const auto& triangle : _iTriangles)
	{
//		if (toucheTriangle(r, t_min, closest_so_far, temp_rec))
//		{
//			hit_anything = true;
//			closest_so_far = temp_rec.t;
//			rec = temp_rec;
//		}
	}

	return hit_anything;
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
