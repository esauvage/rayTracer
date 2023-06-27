#ifndef PHONGBLISS_H
#define PHONGBLISS_H

#include "material.h"

class Scene;

class PhongBliss : public Material
{
public:
    PhongBliss(const Vec3f& a, float phongExp, Scene * scene = nullptr);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
    void setScene(Scene *newScene);

    void setPhongExp(float newPhongExp);

    float phongExp() const;

private:
    float _coeff;
    float _phongExp;
    Scene * _scene;
};

inline void to_json(nlohmann::json& j, const PhongBliss& m) {
    j = nlohmann::json{{"albedo", m.albedo()},{"phong exp", m.phongExp()}};
}

#endif // PHONGBLISS_H
