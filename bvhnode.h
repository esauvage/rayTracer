#ifndef BVHNODE_H
#define BVHNODE_H

#include "shape.h"

class BVHNode : public Shape
{
public:
	BVHNode();
	BVHNode(std::shared_ptr<Shape> object);

	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, std::ofstream& file) const override;
	virtual bool boundingBox(double time0, double time1, Eigen::AlignedBox3f& output_box) const override;
	virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override;
	void add(std::shared_ptr<Shape> object);

private:
	std::shared_ptr<Shape> _left;
	std::shared_ptr<Shape> _right;
	Eigen::AlignedBox3f _aabb;
};

#endif // BVHNODE_H
