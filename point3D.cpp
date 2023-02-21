#include "Vec3f.h"

#include <algorithm>

using namespace std;

Vec3f::Vec3f(float a, float b, float c)
	:x(a), y(b), z(c)
{
}

Vec3f Vec3f::borne(const Vec3f &maxP)
{
	x = min(x, maxP.x);
	y = min(y, maxP.y);
	z = min(z, maxP.z);
	return *this;
}
