#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <Eigen/Geometry>

using Vec3f = Vec<float, 3>;

inline float frand() {return rand()/static_cast<float>(RAND_MAX);}
// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
template <class T>
Eigen::Vector3<T> barycentric(Eigen::Vector3<T> p, Eigen::Vector3<T> a, Eigen::Vector3<T> b, Eigen::Vector3<T> c)
{
	const auto v0 = b - a, v1 = c - a, v2 = p - a;
	const auto d00 = v0.dot(v0);
	const auto d01 = v0.dot(v1);
	const auto d11 = v1.dot(v1);
	const auto d20 = v2.dot(v0);
	const auto d21 = v2.dot(v1);
	const auto denom = d00 * d11 - d01 * d01;
	if (!denom)
	{
		return Eigen::Vector3f(0, 0, 0);
	}
	const auto v = (d11 * d20 - d01 * d21) / denom;
	const auto w = (d00 * d21 - d01 * d20) / denom;
	const auto u = 1.0f - v - w;
	return Eigen::Vector3<T>(u, v, w);
}

template <class T>
Eigen::Vector3<T>& dir2vector2(Eigen::Vector3<T>& out, const Eigen::Vector3<T>& in){
    Eigen::Array3<T> sine = in.array().sin();
    Eigen::Array3<T> cosi = in.array().cos();
	out << cosi.row(0) * cosi.row(1) * in[2],
		   sine.row(0) * cosi.row(1) * in[2],
						 sine.row(1) * in[2];
    out.normalize();
    return out;
}

inline Vec3f random_in_unit_sphere()
{
	Vec3f p;
    return dir2vector2(p, Vec3f((frand() * 2)* M_PI, (frand() -0.5)*M_PI, frand()));
//	while (true)
//	{
//		auto p = Vec3f::Random();
//		if (p.squaredNorm() >= 1) continue;
//		return p;
//	}
}

inline Vec3f random_unit_vector()
{
	Vec3f p;
    return dir2vector2(p, Vec3f((frand() * 2)* M_PI,(frand() - 0.5)*M_PI,  1.));
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
