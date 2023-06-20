#ifndef BVHNODE_H
#define BVHNODE_H

#include "shape.h"
#include "aabb.h"

class BVHNode : public Shape
{
public:
	BVHNode();

	virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
	virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override;
	void add(std::shared_ptr<Shape> object);

private:
	std::shared_ptr<Shape> _left;
	std::shared_ptr<Shape> _right;
    AABB _aabb;
};

#endif // BVHNODE_H
