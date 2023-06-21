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
	void add(const Vec3f & point);
	void add(const Eigen::Vector3i & triangle);
	void update();

private:
	bool toucheTriangle(const Eigen::Vector3i &triangle, const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const;
    Vec3f centre(const Eigen::Vector3i &triangle) const;

	std::vector<Vec3f > _points;
	std::vector<Vec3f > _normales;
	std::vector<Vec2f > _texs;
    std::vector<Eigen::Vector3i > _iTriangles;
    Eigen::Quaternion<float> _rotation{0, 0, 0, 1};
    AABB _aabb;
};

#endif // MESH_H
