#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Geometry>

using Vec3f = Eigen::Vector3f;

class Material
{
public:
	Vec3f col;
	float reflectance;
	float refraction;
	float transparence;
};

#endif // MATERIAL_H
