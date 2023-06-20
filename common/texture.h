#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"

class Texture : public Material
{
public:
    Texture(const Vec3f &albedo);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	std::string file() const;
    virtual nlohmann::json &jsonHelper(nlohmann::json &j) const;
    std::shared_ptr<Material> material1() const;
    void setMaterial1(const std::shared_ptr<Material> &newMaterial);
    std::shared_ptr<Material> material2() const;
    void setMaterial2(const std::shared_ptr<Material> &newMaterial);

private:
    std::shared_ptr<Material>_material1 {nullptr};
    std::shared_ptr<Material>_material2 {nullptr};
};

inline void to_json(nlohmann::json& j, const Texture& m) {
    j = nlohmann::json{{"albedo", m.albedo()}, {"material1", m.material1()->nom()}, {"material2", m.material2()->nom()}};
}

#endif // TEXTURE_H
