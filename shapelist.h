#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "shape.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class ShapeList : public Shape
{
public:
	ShapeList();
	ShapeList(shared_ptr<Shape> object);

	void clear();
	void add(shared_ptr<Shape> object);

	virtual bool touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const override;

	std::vector<shared_ptr<Shape>> objects;
};

#endif // SHAPELIST_H
