#include "shapelist.h"

#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
using namespace Eigen;
using namespace nlohmann;

ShapeList::ShapeList()
{

}

ShapeList::ShapeList(shared_ptr<Shape> object)
{
	add(object);
}

void ShapeList::clear()
{
	_objects.clear();
}

void ShapeList::add(shared_ptr<Shape> object)
{
	_objects.push_back(object);
}

bool ShapeList::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;
	for (const auto& object : _objects)
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

json &ShapeList::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	for (const auto &o : _objects)
    {
        j += json(o);
    }
	return j;
}

bool ShapeList::boundingBox(double time0, double time1, AABB &outputBox) const
{
	outputBox.setEmpty();
	for (const auto &o : _objects)
	{
        AABB aabb;
		o->boundingBox(time0, time1, aabb);
        outputBox = outputBox.merged(aabb);
	}
	return true;
}
