#ifndef AABB_H
#define AABB_H

#include <Eigen/Geometry>
#include "utils.h"

class AABB:public Eigen::AlignedBox3f
{
public:
	AABB();
	virtual bool touche(const Rayon3f& r, double t_min, double t_max) const;
};

#endif // AABB_H
