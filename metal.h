#ifndef METAL_H
#define METAL_H

#include "material.h"

class Metal : public Material
{
public:
	Metal(const Vec3f &albedo, float fuzz = 0);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
private:
	float _fuzz;
};

#endif // METAL_H
