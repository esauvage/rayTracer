#include "bvhnode.h"

#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
using namespace Eigen;
using namespace nlohmann;

BVHNode::BVHNode()
{

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
        _right = object;
		boundingBox(0, 0, _aabb);
		return;
	}
    AABB aabb;
	object->boundingBox(0, 0, aabb);
    AABB aabbLeft;
	_left->boundingBox(0, 0, aabbLeft);
    AABB aabbRight;
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
    if (i == _left)
    {
        _left = p;
    }
    else
    {
        _right = p;
    }
	boundingBox(0, 0, _aabb);
}

bool BVHNode::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
{
    if (!_aabb.touche(r, t_min, t_max))
    {
        return false;
    }
    bool hit_left = _left->touche(r, t_min, t_max, rec);
    bool hit_right = _right->touche(r, t_min, hit_left ? rec.t : t_max, rec);
    return hit_left||hit_right;
}

json &BVHNode::jsonHelper(json &j) const
{
	j = Shape::jsonHelper(j);
    j += json(_left);
    j += json(_right);
	return j;
}

bool BVHNode::boundingBox(double time0, double time1, AABB &outputBox) const
{
	_left->boundingBox(time0, time1, outputBox);
    AABB aabb;
	_right->boundingBox(time0, time1, aabb);
	outputBox = outputBox.extend(aabb);
	return true;
}
