#ifndef LIGHT_H
#define LIGHT_H

#include "material.h"

class Light : public Material
{
public:
    Light(const Vec3f &a, float intensity);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const;
    Vec3f emitted(const Vec2f & tex, const Vec3f &p) const;
    float intensity() const;
    void setIntensity(float newIntensity);

private:
    float _intensity;
};

inline void to_json(nlohmann::json& j, const Light& m) {
    j = nlohmann::json{{"light", m.albedo()}, {"intensity", m.intensity()}};
}

#endif // LIGHT_H
