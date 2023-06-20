#ifndef MESH_H
#define MESH_H

#include "shape.h"

class Mesh : public Shape
{
public:
	Mesh();

	virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
	virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
    void add(std::shared_ptr<Shape> triangle);
	void add(const Vec3f & point);
	void add(const Eigen::Vector3i & triangle);
	void update();

private:
	bool toucheTriangle(const Eigen::Vector3i &triangle, const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const;
    Vec3f centre(const Eigen::Vector3i &triangle) const;

    std::vector<std::shared_ptr<Shape> > _triangles;
	std::vector<Vec3f > _points;
	std::vector<Vec3f > _normales;
	std::vector<Eigen::Vector3i > _iTriangles;
};

#endif // MESH_H
