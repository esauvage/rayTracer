#include "metal.h"

#include "utils.h"

Metal::Metal(const Vec3f &albedo, float fuzz)
	: Material(albedo), _fuzz(fuzz)
{

}

bool Metal::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	const Vec3f reflected = reflect(r_in.direction().normalized(), rec.normal());
	scattered = Rayon3f(rec.p, reflected + _fuzz*random_in_unit_sphere());
	attenuation = albedo();
	bool r = fabs(scattered.direction().dot(rec.normal())) > 1e-5;
	while (!r)
	{
		scattered = Rayon3f(rec.p, reflected + _fuzz*random_in_unit_sphere());
		r = fabs(scattered.direction().dot(rec.normal())) > 1e-5;
	}
	return r;
}
