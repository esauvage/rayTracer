#ifndef LAMBERTIEN_H
#define LAMBERTIEN_H

#include "material.h"

class Lambertien : public Material
{
public:
    Lambertien(const Vec3f& a);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
};

inline void to_json(nlohmann::json& j, const Lambertien& m) {
    j = nlohmann::json{{"albedo", m.albedo()}};
}

#endif // LAMBERTIEN_H
