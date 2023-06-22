#include "mesh.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "utils.h"

using namespace nlohmann;
using namespace std;

Mesh::Mesh()
{

}

bool Mesh::touche(const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const
{
    if (!_root)
        return false;
    return _root->touche(r, t_min, t_max, rec);
}

bool MeshNode::touche(const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const
{
    if (!_aabb.touche(r, t_min, t_max))
        return false;
    if (_i >= 0)
    {
        if (toucheTriangle(_mesh->points(_i), r, t_min, t_max, rec, _i))
        {
            if (rec.p.hasNaN())
            {
                return false;
            }
            return true;
        }
    }
    bool hit_left = _left->touche(r, t_min, t_max, rec);
    bool hit_right = _right->touche(r, t_min, hit_left ? rec.t : t_max, rec);
    return hit_left||hit_right;
}

bool MeshNode::toucheTriangle(const std::vector<Vec3f> &points, const Rayon3f &r, float t_min, float t_max, HitRecord &rec, int index) const
{
    if (!aabb().touche(r, t_min, t_max))
    {
        return false;
    }
    const float EPSILON = 1e-5;
    Rayon3f rLocal = r;
    const Vec3f p[3] {points[0], points[1], points[2]};
    const auto edge1{ points[1] - p[0]};
    const auto edge2{ points[2] - p[0]};
    //	auto _n(edge1.cross(edge2).normalized());
    Vec3f h, s, q;
    float a,f,u,v;
    h = rLocal.direction().cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rLocal.origin() - p[0];
    u = f * s.dot(h);
    if ((u < 0.0) || (u > 1.0))
    {
        return false;
    }
    q = s.cross(edge1);
    v = f * rLocal.direction().dot(q);
    if ((v < 0.0) || (u + v > 1.0))
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    const float t = f * edge2.dot(q);
    if (t < EPSILON || t < t_min || t > t_max) // ray intersection
    {
        return false;
    }
    rec.t = t;
    rec.p = rLocal.pointAt(rec.t);
    //On touche.
    Vec3f rNorm;
    Vec3f baryPoint = barycentric(rec.p, p[0], p[1], p[2]);
    auto normals = _mesh->normals(index);
    for (int i = 0; i < 3; ++i)
    {
        rNorm += normals.at(i) * baryPoint[i];
    }
    //Cette ligne supprime le lissage des normales
    //	rNorm = edge1.cross(edge2);
    //	rNorm.normalize();
    rec.setFaceNormal(rLocal, rNorm.normalized());
    rec.pMaterial = material();
    Vec2f tex;
    auto textures = _mesh->textures(index);
    tex =textures[0] * baryPoint(0) + textures[1] * baryPoint(1) + textures[2] * baryPoint(2);
    rec.setTex(tex);

    return true;
}

int MeshNode::i() const
{
    return _i;
}

//bool Mesh::toucheTriangle(const Eigen::Vector3i &triangle, const Rayon3f &r, float t_min, float t_max, HitRecord &rec) const
//{
//    if (!aabb().touche(r, t_min, t_max))
//    {
//        return false;
//    }
//    const float EPSILON = 1e-5;
//    Rayon3f rLocal = r;
//    rLocal.direction() = _rotation.inverse()*r.direction();
//    rLocal.origin() = _rotation.inverse()*r.origin();
//    const Vec3f _p[3] {_points[triangle[0]], _points[triangle[1]], _points[triangle[2]]};
//	const auto edge1{ _points[triangle[1]] - _p[0]};
//	const auto edge2{ _points[triangle[2]] - _p[0]};
////	auto _n(edge1.cross(edge2).normalized());
//	Vec3f h, s, q;
//	float a,f,u,v;
//    h = rLocal.direction().cross(edge2);
//	a = edge1.dot(h);
//	if (a > -EPSILON && a < EPSILON)
//		return false;    // This ray is parallel to this triangle.
//	f = 1.0/a;
//    s = rLocal.origin() - _p[0];
//	u = f * s.dot(h);
//	if ((u < 0.0) || (u > 1.0))
//	{
//		return false;
//	}
//	q = s.cross(edge1);
//    v = f * rLocal.direction().dot(q);
//	if ((v < 0.0) || (u + v > 1.0))
//		return false;
//	// At this stage we can compute t to find out where the intersection point is on the line.
//	const float t = f * edge2.dot(q);
//	if (t < EPSILON || t < t_min || t > t_max) // ray intersection
//	{
//		return false;
//	}
//	rec.t = t;
//    rec.p = rLocal.pointAt(rec.t);
//	//On touche.
//	Vec3f rNorm;
//	Vec3f baryPoint = barycentric(rec.p, _p[0], _p[1], _p[2]);
//	for (int i = 0; i < 3; ++i)
//	{
//		rNorm += _normals.at(triangle[i]) * baryPoint[i];
//	}
//	//Cette ligne supprime le lissage des normales
////	rNorm = edge1.cross(edge2);
////	rNorm.normalize();
//    rec.setFaceNormal(rLocal, rNorm.normalized());
//    rec.pMaterial = material();
//	Vec2f tex;
//	tex = _texs[triangle[0]] * baryPoint(0) + _texs[triangle[1]] * baryPoint(1) + _texs[triangle[2]] * baryPoint(2);
//	rec.setTex(tex);

//    return true;
//}

Vec3f Mesh::centre(const Eigen::Vector3i &triangle) const
{
	Vec3f r(0., 0., 0.);
    for (const auto p : triangle)
    {
        r += _points.at(p);
    }
    return r/3.;
}

json &Mesh::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	for (const auto &o : _points)
	{
		j["points"] += json(o);
	}
	for (const auto &i : _iTriangles)
	{
		j["itriangles"] += json(i);
	}
	for (const auto &i : _texs)
	{
		j["texs"] += json(i);
	}
	for (const auto &i : _normals)
	{
		j["normals"] += json(i);
	}
	return j;
}

void Mesh::addVertex(const Vec3f &point)
{
	_points.push_back(point);
}

void Mesh::addTex(const Vec2f &tex)
{
	_texs.push_back(tex);
}

void Mesh::addNormal(const Vec3f &normal)
{
	_normals.push_back(normal);
}

void Mesh::addVertI(const Eigen::Vector3i &triangle)
{
    _iTriangles.push_back(triangle);
    if(!_root)
    {
        _root = make_shared<MeshNode>(MeshNode());
        _root->setMesh(this);
    }
    _root->add(_iTriangles.size()-1);
}

//Calcule les normales en chaque point en faisant une moyenne par les angles.
//Source : https://stackoverflow.com/questions/45477806/general-method-for-calculating-smooth-vertex-normals-with-100-smoothness
void Mesh::update()
{
	_normals.assign(_points.size(), Vec3f(0, 0, 0));
	for(size_t i = 0; i < _iTriangles.size(); i++)
	{
		const auto v10 = _points[_iTriangles[i][1]] - _points[_iTriangles[i][0]];
		const auto v20 = _points[_iTriangles[i][2]] - _points[_iTriangles[i][0]];
		const auto v21 = _points[_iTriangles[i][2]] - _points[_iTriangles[i][1]];
		const auto v01 = _points[_iTriangles[i][0]] - _points[_iTriangles[i][1]];
		const auto v12 = _points[_iTriangles[i][1]] - _points[_iTriangles[i][2]];
		const auto v02 = _points[_iTriangles[i][0]] - _points[_iTriangles[i][2]];

		const auto fn = v10.cross(v20).normalized();
		auto a0 = acos(v10.dot(v20)/sqrt(v10.squaredNorm() * v20.squaredNorm()));    // p1 is the 'base' here
		auto a1 = acos(v21.dot(v01)/sqrt(v21.squaredNorm() * v01.squaredNorm()));    // p2 is the 'base' here
		auto a2 = acos(v02.dot(v12)/sqrt(v02.squaredNorm() * v12.squaredNorm()));    // p3 is the 'base' here
		if (a0 != a0)
			a0 = 0.;
		if (a1 != a1)
			a1 = 0.;
		if (a2 != a2)
			a2 = 0.;
		_normals[_iTriangles[i][0]] += fn * a0;
		_normals[_iTriangles[i][1]] += fn * a1;
		_normals[_iTriangles[i][2]] += fn * a2;
	}

	for (auto & n: _normals)
	{
        n = n.normalized();
    }
}

std::vector<Vec3f> Mesh::points(int indexTriangle)
{
    Eigen::Vector3i index = _iTriangles[indexTriangle];
    std::vector<Vec3f> ret;
    for (const auto i : index)
    {
        ret.push_back(_points[i]);
    }
    return ret;
}


std::vector<Vec3f> Mesh::normals(int indexTriangle)
{
    Eigen::Vector3i index = _iTriangles[indexTriangle];
    std::vector<Vec3f> ret;
    for (const auto i : index)
    {
        ret.push_back(_normals[i]);
    }
    return ret;
}

std::vector<Vec2f> Mesh::textures(int indexTriangle)
{
    Eigen::Vector3i index = _iTriangles[indexTriangle];
    std::vector<Vec2f> ret;
    for (const auto i : index)
    {
        ret.push_back(_texs[i]);
    }
    return ret;
}

bool Mesh::boundingBox(double time0, double time1, AABB &outputBox) const
{
    Vec3f min = _points[0];
    Vec3f max = _points[0];
    for (const auto &p : _points)
    {
        min = p.array().min(min.array());
        max = p.array().max(max.array());
    }
    outputBox = AABB(min, max);
    if (_root)
    {
        outputBox = _root->boundingBox();
    }
    return true;
}

MeshNode::MeshNode()
    :_i(-1), _left(nullptr)
{

}

void MeshNode::add(int i)
{
    if (_i < 0)
    {
        _i = i;
        boundingBox();
        return;
    }
    shared_ptr <MeshNode> p = make_shared<MeshNode>(MeshNode());
    p->setMesh(_mesh);
    p->add(i);
    add(p);
}

void MeshNode::add(shared_ptr<MeshNode> p)
{
    if (!_left)
    {
        _left = p;
        _right = p;
        boundingBox();
        return;
    }
    if (_right == _left)
    {
        _right = p;
        boundingBox();
        return;
    }
    AABB aabb;
    p->boundingBox();
    AABB aabbLeft;
    _left->boundingBox();
    AABB aabbRight;
    _right->boundingBox();
    aabbLeft = aabbLeft.extend(aabb);
    aabbRight = aabbRight.extend(aabb);
    shared_ptr <MeshNode> index;
    if (aabbRight.diagonal().squaredNorm() < aabbLeft.diagonal().squaredNorm())
    {
        //insertion à droite
        index = _right;
    }
    else
    {
        //insertion à gauche
        index = _left;
    }
    shared_ptr <MeshNode> insert;
    insert = make_shared<MeshNode>(MeshNode());
    insert->setMesh(_mesh);
    insert->add(index);
    insert->add(p);
    if (index == _left)
    {
        _left = insert;
    }
    else
    {
        _right = insert;
    }
    boundingBox();
}

const AABB &MeshNode::boundingBox()
{
    if (_i >= 0)
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
        return _aabb;
    }
    if (_left)
    {
        _aabb = _left->boundingBox();
        _aabb.merged(_right->boundingBox());
    }
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
