#ifndef SUN_H
#define SUN_H

#include "utils.h"
#include <nlohmann/json.hpp>

class Sun
{
public:
	Sun(const Vec3f &direction, const Vec3f &color, const float intensity);
	Vec3f color() const;
	void setColor(const Vec3f &newColor);

	Vec3f direction() const;
	void setDirection(const Vec3f &newDirection);

	float intensity() const;
	void setIntensity(float newIntensity);

private:
	Vec3f _color;
	Vec3f _direction;
	float _intensity;
};

inline void to_json(nlohmann::json& j, const Sun& sun)
{
    j = nlohmann::json{{"color", sun.color()}, {"intensity", sun.intensity()}};
    if (sun.direction().squaredNorm() == 1)
    {
        j["direction"] = sun.direction();
    }
}

inline void from_json(const nlohmann::json& j, Sun& sun)
{
    Vec3f direction(INFINITY, INFINITY, INFINITY);
    if (j.contains("direction"))
    {
        direction = Vec3f(j.at("direction")[0].get<float>(), j.at("direction")[1].get<float>(), j.at("direction")[2].get<float>());
    }
    sun.setDirection(direction);
	const Vec3f color(j.at("color")[0].get<float>(), j.at("color")[1].get<float>(), j.at("color")[2].get<float>());
	sun.setColor(color);
	sun.setIntensity(j.at("intensity").get<float>());
}

#endif // SUN_H
