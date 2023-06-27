#include "phongbliss.h"

#include "utils.h"
#include "scene.h"

using namespace Eigen;
using namespace std;

PhongBliss::PhongBliss(const Vec3f& a, float phongExp, Scene * scene)
    : Material(a), _scene(scene), _phongExp(phongExp)
{
    _coeff = 1.0;
}

bool PhongBliss::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    if (!_scene)
        return false;
    attenuation = Vec3f(0, 0, 0);
    for (const auto &sun: _scene->suns())
    {
        if (sun.direction().hasNaN()) continue;
        Vec3f h = (- r_in.direction().stableNormalized() + sun.direction());
        h.stableNormalize();
        //Intensity of the specular light
        float NdotH = fmax(0, rec.normal().dot(h));
        float intensity = pow(NdotH, _phongExp);
        if (intensity != intensity) continue;
        //Sum up the specular light factoring
        attenuation += _coeff * sun.color() * intensity;
    }
    return false;
}

void PhongBliss::setScene(Scene *newScene)
{
    _scene = newScene;
}

void PhongBliss::setPhongExp(float newPhongExp)
{
    _phongExp = newPhongExp;
}

float PhongBliss::phongExp() const
{
    return _phongExp;
}
