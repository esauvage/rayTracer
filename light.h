#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Geometry>

using Vec3f = Eigen::Vector3f;

class Light
{
public:
	Light(const Vec3f &position, const Vec3f &couleur);
	Vec3f pos;
	Vec3f col;
};

#endif //LIGHT_H
