#ifndef PHONGBLISS_H
#define PHONGBLISS_H

#include "material.h"

class PhongBliss : public Material
{
public:
    PhongBliss(const Vec3f& a);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
                         Vec3f& attenuation) const override;
private:
    float _coeff;
    float _phongExp;
};

inline void to_json(nlohmann::json& j, const PhongBliss& m) {
    j = nlohmann::json{{"albedo", m.albedo()}};
}

#endif // PHONGBLISS_H
