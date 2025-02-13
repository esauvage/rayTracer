#ifndef UTILS_H
#define UTILS_H

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <Eigen/Geometry>
#include <iostream>
#include <nlohmann/json.hpp>

template<typename Type, int Size>
using Vec = Eigen::Matrix<Type, Size, 1>;
template<typename Type, int Size>
using Rayon = Eigen::ParametrizedLine<Type, Size>;
using Rayon3f = Rayon<double, 3>;

using Vec3f = Vec<double, 3>;
using Vec2f = Vec<double, 2>;
inline double frand() {return rand()/static_cast<double>(RAND_MAX);}

namespace nlohmann {
	template <typename T>
	struct adl_serializer<std::shared_ptr<T> > {
		static void to_json(nlohmann::json& j, const std::shared_ptr<T>& p) {
			j = nlohmann::json(*p);
		}

		static void from_json(const nlohmann::json& j, std::shared_ptr<T>& p) {
				*p = j.get<T>(); // same as above, but with
								  // adl_serializer<T>::from_json
			}
		};
}

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
Eigen::Vector3<T> barycentricEdges(Eigen::Vector3<T> v0, Eigen::Vector3<T> v1, Eigen::Vector3<T> v2)
{
//	const auto v0 = b - a, v1 = c - a, v2 = p - a;
	const auto d00 = v0.dot(v0);
	const auto d01 = v0.dot(v1);
	const auto d11 = v1.dot(v1);
	const auto d20 = v2.dot(v0);
	const auto d21 = v2.dot(v1);
	const auto denom = d00 * d11 - d01 * d01;
	if (!denom || denom!=denom)
	{
        return Eigen::Vector3<T>(0, 0, 0);
	}
	if (d21!=d21)
	{
		std::cout << "v2 : " << v2 << std::endl;
		std::cout << "v1 : " << v1 << std::endl;
        return Eigen::Vector3<T>(0, 0, 0);
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
//	cout << out << endl;
    return out;
}

template <class T>
inline Eigen::Vector3<T> random_in_unit_sphere()
{
    Eigen::Vector3<T> p;
    return dir2vector2(p, Eigen::Vector3<T>((frand() - 0.5) * M_PI, (frand() * 2) * M_PI, frand()));
//	while (true)
//	{
//		auto p = Vec3f::Random();
//		if (p.squaredNorm() >= 1) continue;
//		return p;
//	}
}

template <class T>
inline Eigen::Vector3<T>random_unit_vector()
{
    Eigen::Vector3<T> p;
    p = dir2vector2(p, Eigen::Vector3<T>((frand() - 0.5)*M_PI, (frand() * 2)* M_PI, 1.));
	if (p.hasNaN())
	{
		std::cout << "Erreur random_unit_vector" << std::endl;
	}
	return p;
//	return random_in_unit_sphere().normalized();
}

template <class T>
inline Eigen::Vector3<T> random_in_unit_disk()
{
    Eigen::Vector3<T> p;
    return dir2vector2(p, Eigen::Vector3<T>(frand()*2*M_PI, 0, frand()));
//	return random_in_unit_sphere().normalized();
}

template <class T>
inline Eigen::Vector3<T> random_on_hemisphere(const Eigen::Vector3<T>& normal) {
    Eigen::Vector3<T> on_unit_sphere = random_unit_vector<T>();
    if (on_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

class Material;

class HitRecord {
public:
	HitRecord(){}
	Vec3f p;
    double t;
	bool front_face;
	std::shared_ptr<Material> pMaterial;

	inline void setFaceNormal(const Rayon3f& r, const Vec3f& outwardNormal) {
		front_face = r.direction().dot(outwardNormal) < 0;
		_normal = front_face ? outwardNormal :-outwardNormal;
	}
	Vec3f normal() const;

	Vec2f tex() const;
	void setTex(const Vec2f &newTex);
    void setTex(const double u, const double v);

private:
	Vec3f _normal;
	Vec2f _tex;
};

inline Vec3f HitRecord::normal() const
{
	return _normal;
}

inline Vec2f HitRecord::tex() const
{
	return _tex;
}

inline void HitRecord::setTex(const Vec2f &newTex)
{
	_tex = newTex;
}

inline void HitRecord::setTex(const double u, const double v)
{
	_tex = Vec2f(u, v);
}

#endif // UTILS_H
