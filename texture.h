#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Texture : public Material
{
public:
	Texture(const Vec3f &albedo, const std::string &filename);
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
	std::string file() const;
	virtual json &jsonHelper(json &j) const;
private:
	std::string _file;
};

inline void to_json(json& j, const Texture& m) {
	j = json{{"albedo", m.albedo()}, {"filename", m.file()}};
}

#endif // TEXTURE_H
