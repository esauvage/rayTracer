#include "aabb.h"

using namespace Eigen;

AABB::AABB()
    :AlignedBox<double, 3>()
{

}

AABB::AABB(const AlignedBox<double, 3> &a)
    :AlignedBox<double, 3>(a)
{

}

AABB::AABB(const Vec3f& min, const Vec3f& max)
    :AlignedBox<double, 3>(min, max)
{

}

bool AABB::touche(const Rayon3f& r, float t_min, float t_max) const
{
	for (int a = 0; a < 3; a++) {
		auto invD = 1.0f / r.direction()[a];
		auto t0 = (min()[a] - r.origin()[a]) * invD;
		auto t1 = (max()[a] - r.origin()[a]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		t_min = t0 > t_min ? t0 : t_min;
		t_max = t1 < t_max ? t1 : t_max;
        if (t_max < t_min)
			return false;
	}
	return true;
}

