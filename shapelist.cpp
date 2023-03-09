#include "shapelist.h"

#include "utils.h"
#include <iostream>

using namespace std;

ShapeList::ShapeList()
{

}

ShapeList::ShapeList(std::shared_ptr<Shape> object)
{
	add(object);
}

void ShapeList::clear()
{
	_objects.clear();
}

void ShapeList::add(std::shared_ptr<Shape> object)
{
	_objects.push_back(object);
}

bool ShapeList::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const
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
    for (auto o : _objects)
    {
        j += json(o);
    }
	return j;
}

std::vector<shared_ptr<Shape> > ShapeList::objects() const
{
	return _objects;
}
