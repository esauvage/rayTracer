#ifndef MATERIAL_H
#define MATERIAL_H

#include <random>
#include <Eigen/Geometry>
#include "rayon.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using Vec3f = Eigen::Vector3f;
using Rayon3f = Rayon<float, 3>;

class HitRecord;

class Material
{
public:
	Material(const Vec3f &albedo = Vec3f(0., 0., 0.));
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const = 0;
	Vec3f albedo() const;
	virtual json &jsonHelper(json& j) const;
	std::string nom() const;
	void setNom(const std::string &newNom);

protected:
	Vec3f reflect(const Vec3f& v, const Vec3f& n) const;
private:
	Vec3f _albedo;
	std::string _nom;
};

inline void to_json(json& j, const Material& m)
{
	j = m.jsonHelper(j);
}

#endif // MATERIAL_H
