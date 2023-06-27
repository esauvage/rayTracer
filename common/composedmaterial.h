#ifndef COMPOSEDMATERIAL_H
#define COMPOSEDMATERIAL_H

#include "material.h"

class ComposedMaterial : public Material
{
public:
    ComposedMaterial(const Vec3f& a);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;

private:
    std::vector <std::shared_ptr<Material> > _materials;
};

inline void to_json(nlohmann::json& j, const ComposedMaterial& m) {
    j = nlohmann::json{{"materials", m.albedo()}};
}
};

#endif // COMPOSEDMATERIAL_H
