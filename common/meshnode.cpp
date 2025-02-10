#include "meshnode.h"

#include "mesh.h"
#include <iostream>

using namespace std;

MeshNode::MeshNode()
    :_left(nullptr), _i(-1), _niveau(0)
{

}

bool MeshNode::touche(const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const
{
	if (r.direction().hasNaN())
	{
		std::cout << "Touche::ray direction has NaN" << std::endl;
		return false;
	}
    if (_i >= 0)// Ca c'est pas bon
    {
        return toucheTriangle(r, t_min, t_max, rec);
    }
    if (!_left)
    {
        cout << "Noeud invalide à gauche " << _i << endl;
        return false;
    }
    bool hit_left = _left->touche(r, t_min, t_max, rec);
    if (!_right)
    {
        cout << "Noeud invalide à droite " << _i << endl;
        return hit_left;
    }
    bool hit_right = _right->touche(r, t_min, hit_left ? rec.t : t_max, rec);
    return hit_left||hit_right;
}

bool MeshNode::toucheTriangle(const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const
{
	if (!aabb().touche(r, t_min, t_max))
    {
        return false;
    }
	auto p = _mesh->points(_i);
    const float EPSILON = 1e-5;
//    Rayon3f rLocal = r;
	const Vec3f edge1{ p[1] - p[0]};
	const Vec3f edge2{ p[2] - p[0]};
    Vec3f h, s, q;
    float a,f,u,v;
	h = r.direction().cross(edge2);
    a = edge1.dot(h);
    if ((a > -EPSILON && a < EPSILON)|| (a != a))
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
	s = r.origin() - p[0];
    u = f * s.dot(h);
    if ((u < 0.0) || (u > 1.0))
    {
        return false;
    }
    q = s.cross(edge1);
	v = f * r.direction().dot(q);
    if ((v < 0.0) || (u + v > 1.0))
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    const float t = f * edge2.dot(q);
	if (t < EPSILON || t < t_min || t > t_max || t!=t) // ray intersection
    {
        return false;
    }
    rec.t = t;
	rec.p = r.pointAt(rec.t);
    //On touche.
    Vec3f rNorm;
	Vec3f baryPoint = barycentricEdges(edge1, edge2, Vec3f(rec.p-p[0]));
    auto normals = _mesh->normals(_i);
	for (int i = 0; i < 3; ++i)
    {
        rNorm += normals.at(i) * baryPoint[i];
    }
	if (rNorm.hasNaN())
	{
		cout << "C'est les normales du mesh !" << endl;
		for (int i = 0; i < 3; ++i)
		{
            cout << "norme " << i << " : " << rNorm[i] << endl;
		}
        return false;
	}
	//Cette ligne supprime le lissage des normales
    //	rNorm = edge1.cross(edge2);
    //	rNorm.normalize();
	rec.setFaceNormal(r, rNorm);
	if (rec.normal().hasNaN())
	{
		cout << "C'est la normalisation !" << endl;
		cout << baryPoint << endl;
		return false;
	}
	rec.pMaterial = _mesh->material();
    Vec2f tex;
    auto textures = _mesh->textures(_i);
	tex = textures[0] * baryPoint(0) + textures[1] * baryPoint(1) + textures[2] * baryPoint(2);
    rec.setTex(tex);
	return true;
}

void MeshNode::setRight(const std::shared_ptr<MeshNode> &newRight)
{
	_right = newRight;
}

void MeshNode::setLeft(const std::shared_ptr<MeshNode> &newLeft)
{
	_left = newLeft;
}

int MeshNode::niveau() const
{
    return _niveau;
}

int MeshNode::i() const
{
    return _i;
}

void MeshNode::add(int i)
{
    if (_left || _i >= 0)
    {
        shared_ptr <MeshNode> p = make_shared<MeshNode>(MeshNode());
        p->setMesh(_mesh);
        p->add(i);
        _niveau = max(_niveau, add(p));
        return;
    }
    _i = i;
    return;
}

int MeshNode::add(shared_ptr<MeshNode> p)
{
    if (p->i() < 0)
    {
        cout << "Erreur Noeud a inserer" << endl;
        return 0;
    }
	if (!_left)
    {
        shared_ptr <MeshNode> nouveau = make_shared<MeshNode>(MeshNode());
        nouveau->setMesh(_mesh);
        nouveau->add(_i);
        _i = -1;
        _left = nouveau;
		_right = p;
        return 1;
    }
    AABB aabb;
    aabb = p->boundingBox();
    AABB aabbLeft;
    aabbLeft = _left->boundingBox();
    AABB aabbRight;
    aabbRight = _right->boundingBox();
	AABB aabbBis = aabbLeft;
	aabbBis = aabbBis.extend(aabbRight);
	if (aabb.diagonal().squaredNorm() > aabbBis.diagonal().squaredNorm())
	{
		shared_ptr <MeshNode> nouveau = make_shared<MeshNode>(MeshNode());
		nouveau->setMesh(_mesh);
		nouveau->setLeft(_left);
		nouveau->setRight(_right);
		_left = p;
		_right = nouveau;
		return nouveau->niveau() + 1;
	}
	aabbLeft = aabbLeft.extend(aabb);
    aabbRight = aabbRight.extend(aabb);
    shared_ptr <MeshNode> index;
    if (aabbRight.diagonal().squaredNorm() < aabbLeft.diagonal().squaredNorm())
    {
        index = _right;
    }
    else
    {
        index = _left;
    }
    return 1 + index->add(p);
//    boundingBox();
}

const AABB &MeshNode::boundingBox()
{
    if (_i >= 0)
    {
		if (_aabb.isEmpty())
		{
			std::vector<Vec3f> points = _mesh->points(_i);
			Vec3f min = points[0];
			Vec3f max = points[0];
			for (const auto &p : points)
			{
				min = p.array().min(min.array());
				max = p.array().max(max.array());
			}
			_aabb = AABB(min, max);
		}
        return _aabb;
    }
    _aabb = _left->boundingBox();
    _aabb.merged(_right->boundingBox());
    return _aabb;
}

void MeshNode::setMesh(Mesh *newMesh)
{
    _mesh = newMesh;
}

AABB MeshNode::aabb() const
{
    return _aabb;
}
