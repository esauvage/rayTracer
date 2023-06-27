#include "phongbliss.h"

#include "utils.h"
#include "scene.h"

using namespace Eigen;
using namespace std;

PhongBliss::PhongBliss(const Vec3f& a)
	: Material(a), _scene(nullptr)
{
	_phongExp = 10;
	_coeff = 1.0;
}

bool PhongBliss::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	if (!_scene)
		return false;
	attenuation = albedo();
	for (const auto &sun: _scene->suns())
	{
		if (sun.direction().hasNaN()) continue;
		Vec3f h = (- r_in.direction() + sun.direction())/(sun.direction() - r_in.direction()).norm();
		//Intensity of the specular light
		float NdotH = rec.normal().dot(h);
		float intensity = pow(NdotH, _phongExp);

		//Sum up the specular light factoring
		attenuation += _coeff * sun.color() * intensity;
	}
    return true;
}

void PhongBliss::setScene(Scene *newScene)
{
	_scene = newScene;
}
