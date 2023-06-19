#ifndef ALBEDO_H
#define ALBEDO_H

#include "utils.h"

class Albedo
{
public:
	Albedo();
	Vec3f color(const Vec2f &tex, const Vec3f &p);
	virtual json &jsonHelper(json& j) const;
	std::string nom() const;
	void setNom(const std::string &newNom);
private:
	Vec3f _color;
	std::string _nom;
};

inline void to_json(json& j, const Albedo& m)
{
	j = m.jsonHelper(j);
}


#endif // ALBEDO_H
