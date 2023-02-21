#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

#include <array>

using namespace std;

class Triangle : public Shape 
{
public:
	Triangle(const array< Vec3f, 3>& points);
	virtual float distance(const Rayon3f & r, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
private:
	array< Vec3f, 3 > _p;
	Vec3f _n;
	Vec3f _e1;
	Vec3f _e2;
};

#endif //TRIANGLE_H
