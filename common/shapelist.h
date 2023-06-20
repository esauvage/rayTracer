#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "shape.h"

#include <vector>

class ShapeList : public Shape
{
public:
	ShapeList();
	ShapeList(std::shared_ptr<Shape> object);

	virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const override;
	void add(std::shared_ptr<Shape> object);

private:
	void clear();

	std::vector<std::shared_ptr<Shape> > _objects;
};

#endif // SHAPELIST_H
