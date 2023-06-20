#include "texture.h"

#include "utils.h"

#include <iostream>

using namespace std;
using namespace nlohmann;

Texture::Texture(const Vec3f &albedo)
    : Material(albedo)
{

}

bool Texture::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	Vec3f scatter_direction = rec.normal() + random_unit_vector();
	// Catch degenerate scatter direction
	auto degenere = 0;
	for (auto x : scatter_direction)
	{
		degenere += fabs(x) > 1e-5 ? 0 : 1;
	}
	if (degenere > 2)
	{
		scatter_direction = rec.normal();
	}
	scattered = Rayon3f(rec.p, scatter_direction.normalized());
	auto sines = sin(10*rec.p.x())*sin(10*rec.p.y())*sin(10*rec.p.z());
    if (_material1 && _material2)
    {
        if (sines > 0)
        {
            _material1->scatter(r_in, rec, attenuation, scattered);
        }
        else
        {
            _material2->scatter(r_in, rec, attenuation, scattered);
        }
    }
    else
    {
        attenuation = (sines > 0) ? albedo() : Vec3f(0, 0, 0);
    }
	return true;
}

json &Texture::jsonHelper(json &j) const
{
	j = Material::jsonHelper(j);
    if (_material1)
    {
        j["material1"] = _material1->nom();
    }
    if (_material2)
    {
        j["material2"] = _material2->nom();
    }
    return j;
}

std::shared_ptr<Material> Texture::material1() const
{
    return _material1;
}

void Texture::setMaterial1(const std::shared_ptr<Material> &newMaterial)
{
    _material1 = newMaterial;
}

std::shared_ptr<Material> Texture::material2() const
{
    return _material2;
}

void Texture::setMaterial2(const std::shared_ptr<Material> &newMaterial)
{
    _material2 = newMaterial;
}

