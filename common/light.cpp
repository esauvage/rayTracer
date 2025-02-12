#include "light.h"

#include "utils.h"

using namespace Eigen;

Light::Light(const Vec3f& a, float intensity)
 : Material(a),
    _intensity(intensity)
{

}

bool Light::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    (void)scattered;
    (void)attenuation;
    (void)r_in;
    (void)rec;
    return false;
}

Vec3f Light::emitted(const Vec2f & tex, const Vec3f& p) const
{
    (void)tex;
    (void)p;
    return albedo() * _intensity;
}

float Light::intensity() const
{
    return _intensity;
}

void Light::setIntensity(float newIntensity)
{
    _intensity = newIntensity;
}
