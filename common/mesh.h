#ifndef MESH_H
#define MESH_H

#include "shape.h"

class Mesh;
class MeshNode : public Shape
{
public:
    MeshNode();
    virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override {return j;}
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override {output_box = _aabb; return true;}
    void add(int i);
    void add(std::shared_ptr<MeshNode> p);
    const AABB & boundingBox();

    void setMesh(Mesh *newMesh);

    AABB aabb() const;

    int i() const;

private:
    bool toucheTriangle(const std::vector<Vec3f> &points, const Rayon3f &r, float t_min, float t_max, HitRecord &rec, int index) const;
    std::shared_ptr<MeshNode> _left;
    std::shared_ptr<MeshNode> _right;

    int _i;
    Mesh * _mesh;
    AABB _aabb;
};

class Mesh : public Shape
{
public:
	Mesh();

	virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
	virtual nlohmann::json &jsonHelper( nlohmann::json& j) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
	void addVertex(const Vec3f & point);
	void addTex(const Vec2f & tex);
	void addNormal(const Vec3f & normal);
	void addVertI(const Eigen::Vector3i & triangle);
	void update();
    void clearNormals();
    std::vector<Vec3f > points(int indexTriangle);
    std::vector<Vec3f > normals(int indexTriangle);
    std::vector<Vec2f > textures(int indexTriangle);

private:
//	bool toucheTriangle(const Eigen::Vector3i &triangle, const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const;
    Vec3f centre(const Eigen::Vector3i &triangle) const;

	std::vector<Vec3f > _points;
	std::vector<Vec3f > _normals;
	std::vector<Vec2f > _texs;
	std::vector<Eigen::Vector3i > _iTriangles;
	std::vector<Eigen::Vector3i > _iTexs;
	Eigen::Quaternion<float> _rotation{0, 0, 0, 1};
    std::shared_ptr<MeshNode> _root;
};

#endif // MESH_H
