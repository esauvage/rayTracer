#ifndef AABB_H
#define AABB_H

#include <Eigen/Geometry>
#include "utils.h"

class AABB:public Eigen::AlignedBox<double, 3>
{
public:
	AABB();
    AABB(const Eigen::AlignedBox<double, 3> &a);
    AABB(const Vec3f& min, const Vec3f& max);
    virtual bool touche(const Rayon3f& r, float t_min, float t_max) const;
};

#endif // AABB_H
