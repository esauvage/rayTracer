#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include <Eigen/Geometry>

using Vec3f = Eigen::Vector3f;

template <typename T>
class Quaternion
{
public:
	Quaternion(T a = 0, T b = 0, T c = 0, T d = 0): a{a}, b{b}, c{c}, d{d}{}
	static Quaternion<T> fromEuler(T pitch, T roll, T yaw);
	Vec3f rotate(const Vec3f &v)
	{
	//z = a + bi + cj + dk
		auto t2 {a*b};
		auto t3 {a*c};
		auto t4 {a*d};
		auto t5 {-b*b};
		auto t6 {b*c};
		auto t7 {b*d};
		auto t8 {-c*c};
		auto t9 {c*d};
		auto t10 {-d*d};
		float v1new = 2*( (t8 + t10)*v.x() + (t6 -  t4)*v.y() + (t3 + t7)*v.z() ) + v.x();
		float v2new = 2*( (t4 +  t6)*v.x() + (t5 + t10)*v.y() + (t9 - t2)*v.z() ) + v.y();
		float v3new = 2*( (t7 -  t3)*v.x() + (t2 +  t9)*v.y() + (t5 + t8)*v.z() ) + v.z();
		return Vec3f{v1new, v2new, v3new};
	}
	
	T a {0};
	T b {0};
	T c {0};
	T d {0};
};

template<typename T>
Quaternion<T> Quaternion<T>::fromEuler(T yaw, T pitch, T roll)
{
	Quaternion<T> q;
	const T t0 = std::cos(yaw * 0.5);
	const T t1 = std::sin(yaw * 0.5);
	const T t2 = std::cos(roll * 0.5);
	const T t3 = std::sin(roll * 0.5);
	const T t4 = std::cos(pitch * 0.5);
	const T t5 = std::sin(pitch * 0.5);

	q.a = t0 * t2 * t4 + t1 * t3 * t5;
	q.b = t0 * t3 * t4 - t1 * t2 * t5;
	q.c = t0 * t2 * t5 + t1 * t3 * t4;
	q.d = t1 * t2 * t4 - t0 * t3 * t5;
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;
	return q;
}

#endif // QUATERNION_H
