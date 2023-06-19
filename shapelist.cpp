#include "shapelist.h"

#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
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

bool ShapeList::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, ofstream& file) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;
	int i = 0;
	int toucheId = i;
	for (const auto& object : _objects)
	{
		if (object->touche(r, t_min, closest_so_far, temp_rec, file))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
			toucheId = i;
		}
		i++;
	}

//	file << "Touche " << toucheId << endl;
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
