#include "dielectrique.h"

#include "utils.h"
using namespace std;
using namespace nlohmann;

Dielectrique::Dielectrique(float iRefrac) : Material(),
	_iRefrac(iRefrac)
{}

bool Dielectrique::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    (void)attenuation;
	float refraction_ratio = rec.front_face ? (1.0/_iRefrac) : _iRefrac;

	Vec3f unit_direction = r_in.direction();
	float cosTheta = fmin(unit_direction.dot(-rec.normal()), 1.0);
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	bool cannot_refract = refraction_ratio * sinTheta > 1.0;
	Vec3f direction;

	if (cannot_refract || reflectance(cosTheta, refraction_ratio) > frand())
		direction = reflect(unit_direction, rec.normal());
	else
		direction = refract(unit_direction, rec.normal(), refraction_ratio);
	if (direction.hasNaN())
	{
		return false;
	}
	scattered = Rayon3f(rec.p, direction.normalized());
	return true;
}

Vec3f Dielectrique::refract(const Vec3f& uv, const Vec3f& n, float etai_over_etat) const
{
	float cos_theta = fmin(uv.dot(-n), 1.0);
	Vec3f r_out_perp =  etai_over_etat * (uv + cos_theta*n);
	Vec3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * n;
	return r_out_perp + r_out_parallel;
}

float Dielectrique::iRefrac() const
{
	return _iRefrac;
}

json &Dielectrique::jsonHelper(json &j) const
{
	j = Material::jsonHelper(j);
	j["refractiveIndex"] = _iRefrac;
	return j;
}

bool Dielectrique::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &localAttenuation, vector<Rayon3f> &vScattered, Vec3f &attenuation) const
{
	auto degenere = 0;
	for (auto x : attenuation)
	{
		degenere += fabs(x) > 1e-2 ? 0 : 1;
	}
	if (degenere > 2)
	{
		return false;
	}
	bool ret{false};
//	Vec3f color(0., 0., 0.);
	for (int i = 0; i < 5; ++i)
	{
		Rayon3f scattered;
		if (scatter(r_in, rec, localAttenuation, scattered))
		{
			ret = true;
			vScattered.push_back(scattered);
		}
	}
	attenuation = attenuation*0.9;
	return ret;
}
