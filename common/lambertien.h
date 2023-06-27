#ifndef LAMBERTIEN_H
#define LAMBERTIEN_H

#include "material.h"

class Scene;

class Lambertien : public Material
{
public:
	Lambertien(const Vec3f& a);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
//	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
//                         Vec3f& attenuation) const override;
    void setScene(Scene *newScene);

private:
    Scene * _scene;
};

inline void to_json(nlohmann::json& j, const Lambertien& m) {
    j = nlohmann::json{{"albedo", m.albedo()}};
}

#endif // LAMBERTIEN_H
