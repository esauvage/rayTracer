#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <Eigen/Geometry>

using namespace Eigen;

inline float frand() {return rand()/static_cast<float>(RAND_MAX);}
template <class T>
Vector3<T>& dir2vector2(Vector3<T>& out, const Vector3<T>& in){
	Array3<T> sine = in.array().sin();
	Array3<T> cosi = in.array().cos();
	out << cosi.row(0) * cosi.row(1) * in[2],
		   sine.row(0) * cosi.row(1) * in[2],
						 sine.row(1) * in[2];
	return out;
}

inline Vec3f random_in_unit_sphere()
{
	while (true)
	{
		auto p = Vec3f::Random();
		if (p.squaredNorm() >= 1) continue;
		return p;
	}
}

inline Vec3f random_unit_vector()
{
	Vec3f p;
	return dir2vector2(p, Vec3f((frand() - 0.5)*M_PI, (frand() * 2 - 1.)* M_PI, 1.));
//	return random_in_unit_sphere().normalized();
}

inline Vec3f random_in_unit_disk()
{
	Vec3f p;
	return dir2vector2(p, Vec3f(frand()*2*M_PI, 0, frand()));
//	return random_in_unit_sphere().normalized();
}

class Material;

class HitRecord {
public:
	HitRecord(){}
	Vec3f p;
	float t;
	bool front_face;
	std::shared_ptr<Material> pMaterial;

	inline void setFaceNormal(const Rayon3f& r, const Vec3f& outwardNormal) {
		front_face = r.direction().dot(outwardNormal) < 0;
		_normal = front_face ? outwardNormal :-outwardNormal;
	}
	Vec3f normal() const;

private:
	Vec3f _normal;
};

inline Vec3f HitRecord::normal() const
{
	return _normal;
}

#endif // UTILS_H
