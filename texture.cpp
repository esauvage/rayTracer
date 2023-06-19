#include "texture.h"

#include "utils.h"

#include <iostream>

using namespace std;

Texture::Texture(const Vec3f &albedo, const string &filename)
	: Material(albedo), _file(filename)
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
	attenuation = (sines > 0) ? albedo() : Vec3f(0, 0, 0);
	return true;
}

json &Texture::jsonHelper(json &j) const
{
	j = Material::jsonHelper(j);
	j["filename"] = _file;
	return j;
}

