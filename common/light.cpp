#include "light.h"

#include "utils.h"
#include "scene.h"

using namespace Eigen;
using namespace std;

Light::Light(const Vec3f& a, float intensity)
 : Material(a),
    _intensity(intensity)
{

}

bool Light::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    return false;
}

Vec3f Light::emitted(const Vec2f & tex, const Vec3f& p) const
{
    return albedo() * 100;
}

float Light::intensity() const
{
    return _intensity;
}

void Light::setIntensity(float newIntensity)
{
    _intensity = newIntensity;
}
