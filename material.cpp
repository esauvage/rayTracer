#include "material.h"


Material::Material(const Vec3f &albedo) : _albedo(albedo)
{
}

Vec3f Material::albedo() const
{
	return _albedo;
}

json &Material::jsonHelper(json &j) const
{
	j = json{"albedo", _albedo};
	return j;
}

Vec3f Material::reflect(const Vec3f &v, const Vec3f &n) const
{
	return v - 2*v.dot(n)*n;
}
