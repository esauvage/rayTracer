#include "material.h"

using namespace std;

Material::Material(const Vec3f &albedo) : _albedo(albedo)
{
}

Vec3f Material::albedo() const
{
	if (_albedoPtr)
	{
		_albedoPtr->color(tex, p);
	}
	return _albedo;
}

json &Material::jsonHelper(json &j) const
{
	if (_albedo)
		j = json{{"albedo", _albedo->nom()}, {"nom", _nom}};
	else
		j = json{{"albedo", _albedo}, {"nom", _nom}};
	return j;
}

Vec3f Material::reflect(const Vec3f &v, const Vec3f &n) const
{
	return v - 2*v.dot(n)*n;
}

std::string Material::nom() const
{
	return _nom;
}

void Material::setNom(const std::string &newNom)
{
	_nom = newNom;
}

bool Material::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &localAttenuation, vector<Rayon3f> &vScattered, Vec3f &attenuation) const
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
	Rayon3f scattered;
	vScattered.clear();
	bool r = scatter(r_in, rec, localAttenuation, scattered);
	vScattered.push_back(scattered);
	attenuation = attenuation.cwiseProduct(localAttenuation);
	return r;
}
