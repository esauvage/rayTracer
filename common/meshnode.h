#ifndef MESHNODE_H
#define MESHNODE_H

#include "aabb.h"

class Mesh;

class MeshNode
{
public:
    MeshNode();
    virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const;
    void add(int i);
    int add(std::shared_ptr<MeshNode> p);
    const AABB & boundingBox();
    void setMesh(Mesh *newMesh);
    AABB aabb() const;
    int i() const;
    int niveau() const;
	void setLeft(const std::shared_ptr<MeshNode> &newLeft);

	void setRight(const std::shared_ptr<MeshNode> &newRight);

private:
    bool toucheTriangle(const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const;
    std::shared_ptr<MeshNode> _left;
    std::shared_ptr<MeshNode> _right;

    int _i;
    Mesh * _mesh;
    AABB _aabb;
    int _niveau;
};

#endif // MESHNODE_H
