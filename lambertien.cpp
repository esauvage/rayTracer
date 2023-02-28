#include "lambertien.h"

#include "shape.h"

using namespace Eigen;

template <class T>
void dir2vector2(Vector3<T>& out, const Vector3<T>& in){
	Array3<T> sine = in.array().sin();
	Array3<T> cosi = in.array().cos();
	out << cosi.row(0) * cosi.row(1) * in[2],
		   sine.row(0) * cosi.row(1) * in[2],
						 sine.row(1) * in[2];
}
float lfrand() {return rand()/static_cast<float>(RAND_MAX);}

Vec3f random_unit_vector()
{
	Vec3f p;
	dir2vector2(p, Vec3f((lfrand() - 0.5)*M_PI, (lfrand() * 2 - 1.)* M_PI, 1.));
	return p;
}

Lambertien::Lambertien(const Vec3f& a)
 : Material(a)
{

}

bool Lambertien::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	Vec3f scatter_direction = rec.normal + random_unit_vector();
	// Catch degenerate scatter direction
	auto degenere = 0;
	for (auto x : scatter_direction)
	{
		degenere += fabs(x) > 1e-8 ? 0 : 1;
	}
	if (degenere > 2)
	{
		scatter_direction = rec.normal;
	}
	scattered = Rayon3f(rec.p, scatter_direction, nullptr);
	attenuation = albedo();
	return true;
}
