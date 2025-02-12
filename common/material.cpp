#include "material.h"

using namespace std;
using json = nlohmann::json;

Material::Material(const Vec3f &albedo) : _albedo(albedo)
{
}

Vec3f Material::albedo() const
{
	return _albedo;
}

json &Material::jsonHelper(json &j) const
{
    j = json{{"albedo", _albedo}, {"nom", _nom}};
	return j;
}

Vec3f Material::reflect(const Vec3f &v, const Vec3f &n) const
{
	return v - 2*v.dot(n)*n;
}

string Material::nom() const
{
	return _nom;
}

void Material::setNom(const string &newNom)
{
	_nom = newNom;
}

Vec3f Material::emitted(const Vec2f& tex, const Vec3f &p) const
{
    (void)tex;
    (void)p;
    return Vec3f(0,0,0);
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
	vScattered.clear();
    bool ret{false};
    for (int i = 0; i < 10; ++i)
    {
        Rayon3f scattered;
        if (scatter(r_in, rec, localAttenuation, scattered))
        {
            ret = true;
            vScattered.push_back(scattered);
        }
    }
    attenuation = attenuation.cwiseProduct(localAttenuation);
    return ret;
}
