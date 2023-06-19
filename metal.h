#ifndef METAL_H
#define METAL_H

#include "material.h"

class Metal : public Material
{
public:
	Metal(const Vec3f &albedo, float fuzz = 0);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
						 Vec3f& attenuation) const override;
	float fuzz() const;
    virtual nlohmann::json &jsonHelper(nlohmann::json &j) const override;
private:
	float _fuzz;
};

inline void to_json(nlohmann::json& j, const Metal& m) {
    j = nlohmann::json{{"albedo", m.albedo()}, {"fuzz", m.fuzz()}};
}

#endif // METAL_H
