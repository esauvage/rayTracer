#ifndef METAL_H
#define METAL_H

#include "material.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Metal : public Material
{
public:
	Metal(const Vec3f &albedo, float fuzz = 0);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
						 Vec3f& attenuation) const override;
	float fuzz() const;
	virtual json &jsonHelper(json &j) const;
private:
	float _fuzz;
};

inline void to_json(json& j, const Metal& m) {
	j = json{{"albedo", m.albedo()}, {"fuzz", m.fuzz()}};
}

#endif // METAL_H
