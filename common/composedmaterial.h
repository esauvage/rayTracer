#ifndef COMPOSEDMATERIAL_H
#define COMPOSEDMATERIAL_H

#include "material.h"
#include "utils.h"

class Scene;

class ComposedMaterial : public Material
{
public:
    ComposedMaterial(const Vec3f& a);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	virtual nlohmann::json &jsonHelper(nlohmann::json &j) const;
	void addMaterial(std::shared_ptr<Material> material);
	std::vector<std::shared_ptr<Material> > materials() const;

	void setScene(Scene *scene);
private:
    std::vector <std::shared_ptr<Material> > _materials;
};

inline void to_json(nlohmann::json& j, const ComposedMaterial& m) {
	j = nlohmann::json{{"albedo", m.albedo()}, {"materials", m.materials()}};
}


#endif // COMPOSEDMATERIAL_H
