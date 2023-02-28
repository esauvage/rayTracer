#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Geometry>
#include "rayon.h"

using Vec3f = Eigen::Vector3f;
using Rayon3f = Rayon<float, 3>;

class HitRecord;

class Material
{
public:
	Material(const Vec3f &albedo);

	float reflectance;
	float refraction;
	float transparence;
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const = 0;
	Vec3f albedo() const;

private:
	Vec3f _albedo;
};

inline Vec3f Material::albedo() const
{
	return _albedo;
}

inline Material::Material(const Vec3f &albedo) : _albedo(albedo)
{}

#endif // MATERIAL_H
