#ifndef MESH_H
#define MESH_H

#include "shape.h"

class Mesh : public Shape
{
public:
	Mesh();

	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const override;
	virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override;
	void add(std::shared_ptr<Shape> triangle);

private:
	std::vector<std::shared_ptr<Shape> > _triangles;
	std::vector<Vec3f > _points;
};

#endif // MESH_H
