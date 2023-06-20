#include "bvhnode.h"

BVHNode::BVHNode()
{

}

#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
using namespace Eigen;
using namespace nlohmann;

BVHNode::BVHNode(shared_ptr<Shape> object)
{
	add(object);
}

void BVHNode::add(shared_ptr<Shape> object)
{
	if (!_left)
	{
		_left = object;
		_right = object;
		boundingBox(0, 0, _aabb);
		return;
	}
	if (_right == _left)
	{
		_right == object;
		boundingBox(0, 0, _aabb);
		return;
	}
	AlignedBox3f aabb;
	object->boundingBox(0, 0, aabb);
	AlignedBox3f aabbLeft;
	_left->boundingBox(0, 0, aabbLeft);
	AlignedBox3f aabbRight;
	_right->boundingBox(0, 0, aabbRight);
	aabbLeft = aabbLeft.extend(aabb);
	aabbRight = aabbRight.extend(aabb);
	shared_ptr <BVHNode> p;
	shared_ptr <Shape> i;
	if (aabbRight.diagonal().squaredNorm() < aabbLeft.diagonal().squaredNorm())
	{
		//insertion à droite
		i = _right;
	}
	else
	{
		//insertion à gauche
		i = _left;
	}
	p = dynamic_pointer_cast<BVHNode>(i);
	if (!p)
	{
		p = make_shared<BVHNode>(BVHNode());
		p->add(i);
	}
	p->add(object);
	boundingBox(0, 0, _aabb);
}

bool BVHNode::touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, ofstream& file) const
{
//	if (!_aabb.touche())
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;
	if (_left->touche(r, t_min, closest_so_far, temp_rec, file))
	{
		hit_anything = true;
		closest_so_far = temp_rec.t;
		rec = temp_rec;
	}
	if (_right->touche(r, t_min, closest_so_far, temp_rec, file))
	{
		hit_anything = true;
		rec = temp_rec;
	}
	return hit_anything;
}

json &BVHNode::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
	j["left"] = json(_left);
	j["right"] = json(_right);
	return j;
}

bool BVHNode::boundingBox(double time0, double time1, AlignedBox3f &outputBox) const
{
	_left->boundingBox(time0, time1, outputBox);
	AlignedBox3f aabb;
	_right->boundingBox(time0, time1, aabb);
	outputBox = outputBox.extend(aabb);
	return true;
}
