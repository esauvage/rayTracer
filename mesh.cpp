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

	return hit_anything;
}

json &Mesh::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	for (auto o : _triangles)
	{
		j += json(o);
	}
	return j;
}

void Mesh::add(shared_ptr<Shape> triangle)
{
	_triangles.push_back(triangle);
}
