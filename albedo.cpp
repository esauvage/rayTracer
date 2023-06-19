#include "albedo.h"

Albedo::Albedo(const Vec3f &color) : _color(color)
{

}

json &Material::jsonHelper(json &j) const
{
	j = json{{"color", _color}, {"nom", _nom}};
	return j;
}

Vec3f Albedo::color(const Vec2f &tex, const Vec3f &p)
{
	return _color;
}

std::string Albedo::nom() const
{
	return _nom;
}

void Albedo::setNom(const std::string &newNom)
{
	_nom = newNom;
}
