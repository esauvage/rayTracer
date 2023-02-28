#include "metal.h"

#include "shape.h"

Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
	return v - 2*v.dot(n)*n;
}

Metal::Metal(const Vec3f &albedo) : Material(albedo)
{

}

bool Metal::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	Vec3f reflected = reflect(r_in.direction().normalized(), rec.normal);
	scattered = Rayon3f(rec.p, reflected, nullptr);
	attenuation = albedo();
	return (scattered.direction().dot(rec.normal) > 0);
}
