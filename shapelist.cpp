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

bool _prems = true;
bool ShapeList::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	int i = 0;
	for (const auto& object : _objects)
	{
		if (object->touche(r, t_min, closest_so_far, temp_rec))
		{
			if (_prems)
			{
				cout << "objet touche : " << i  << " " << object->nom() << endl;
			}
			_prems = false;
			++i;
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

std::vector<shared_ptr<Shape> > ShapeList::objects() const
{
	return _objects;
}
