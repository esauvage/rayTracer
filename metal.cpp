#include "metal.h"

#include "utils.h"

#include <iostream>

using namespace std;

Metal::Metal(const Vec3f &albedo, float fuzz)
	: Material(albedo), _fuzz(fuzz)
{

}

bool Metal::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	const Vec3f reflected = reflect(r_in.direction(), rec.normal());
	attenuation = albedo(rec.tex(), rec.p);
	bool r = false;//fabs(scattered.direction().dot(rec.normal())) > 1e-5;
	int compteur = 100;
	while (!r && compteur)
	{
		--compteur;
		Vec3f random = (reflected + _fuzz * random_in_unit_sphere()).normalized();
		scattered = Rayon3f(rec.p, random);
		r = fabs(scattered.direction().dot(rec.normal())) > 1e-5;
	}
//	cout << "Scatter direction : " << scattered.direction().x() << " " << scattered.direction().y() << " " << scattered.direction().z();
//	cout << "position"  << scattered.origin().x() << " " << scattered.origin().y() << " " << scattered.origin().z() << endl;
	return r;
}

float Metal::fuzz() const
{
	return _fuzz;
}

json &Metal::jsonHelper(json &j) const
{
	j = Material::jsonHelper(j);
	j["fuzz"] = _fuzz;
	return j;
}

bool Metal::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &localAttenuation, vector<Rayon3f> &vScattered, Vec3f &attenuation) const
{
	auto degenere = 0;
	for (auto x : attenuation)
	{
		degenere += fabs(x) > 1e-2 ? 0 : 1;
	}
	if (degenere > 2)
	{
		return false;
	}
	bool ret{false};
	for (int i = 0; i < 4; ++i)
	{
		Rayon3f scattered;
		if (scatter(r_in, rec, localAttenuation, scattered))
		{
			ret = true;
			vScattered.push_back(scattered);
		}
	}
	attenuation = attenuation.cwiseProduct(localAttenuation);
	return ret;
}
