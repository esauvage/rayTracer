#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "shape.h"

#include <vector>

class ShapeList : public Shape
{
public:
	ShapeList();
	ShapeList(std::shared_ptr<Shape> object);

	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, std::ofstream& file) const override;
	virtual  nlohmann::json &jsonHelper( nlohmann::json& j) const override;
	void add(std::shared_ptr<Shape> object);

private:
	void clear();

	std::vector<std::shared_ptr<Shape> > _objects;
};

#endif // SHAPELIST_H
