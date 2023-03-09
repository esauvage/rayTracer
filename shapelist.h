#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "shape.h"

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using std::shared_ptr;
using std::make_shared;

class ShapeList : public Shape
{
public:
	ShapeList();
	ShapeList(shared_ptr<Shape> object);

	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual json &jsonHelper(json& j) const override;
	void clear();
	void add(shared_ptr<Shape> object);
	std::vector<shared_ptr<Shape> > objects() const;
private:
	std::vector<shared_ptr<Shape> > _objects;
};

#endif // SHAPELIST_H
