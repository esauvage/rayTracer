#ifndef DIELECTRIQUE_H
#define DIELECTRIQUE_H

#include "material.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Dielectrique : public Material
{
public:
	Dielectrique(float iRefrac);

	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
						 Vec3f& attenuation) const override;
	json &jsonHelper(json &j) const;

private:
	float iRefrac() const;
	Vec3f refract(const Vec3f &uv, const Vec3f &n, float etai_over_etat) const;
	static float reflectance(float cosine, float ref_idx) {
		// Use Schlick's approximation for reflectance.
		float r0 = (1-ref_idx) / (1+ref_idx);
		r0 *= r0;
		return r0 + (1-r0)*pow((1 - cosine),5);
	}

	float _iRefrac; // Index of Refraction
};

#endif // DIELECTRIQUE_H
