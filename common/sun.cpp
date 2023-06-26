#include "sun.h"

Sun::Sun(const Vec3f &direction, const Vec3f &color, const float intensity)
	:_direction(direction.normalized()), _color(color), _intensity(intensity)
{

}

Vec3f Sun::color() const
{
	return _color;
}

void Sun::setColor(const Vec3f &newColor)
{
	_color = newColor;
}

Vec3f Sun::direction() const
{
	return _direction;
}

void Sun::setDirection(const Vec3f &newDirection)
{
	_direction = newDirection.normalized();
}

float Sun::intensity() const
{
	return _intensity;
}

void Sun::setIntensity(float newIntensity)
{
	_intensity = newIntensity;
}
