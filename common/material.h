#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Geometry>
#include "rayon.h"
#include <nlohmann/json.hpp>

using Vec3f = Eigen::Vector3f;
using Vec2f = Eigen::Vector2f;

class HitRecord;

class Material
{
public:
	Material(const Vec3f &albedo = Vec3f(0., 0., 0.));
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const = 0;
	virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
						 Vec3f& attenuation) const;
    Vec3f albedo() const;
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const;
	std::string nom() const;
	void setNom(const std::string &newNom);
    virtual Vec3f emitted(const Vec2f& tex, const Vec3f& p) const;

    int nbScatteredRays() const;
    void setNbScatteredRays(int newNbScatteredRays);

protected:
	Vec3f reflect(const Vec3f& v, const Vec3f& n) const;
private:
	Vec3f _albedo;
	std::string _nom;
    int _nbScatteredRays;
};

inline void to_json(nlohmann::json& j, const Material& m)
{
	j = m.jsonHelper(j);
}

#endif // MATERIAL_H
