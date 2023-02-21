#ifndef POINT_3D_H
#define POINT_3D_H

#include <cmath>

class Vec3f
{
public:
	float x, y, z;  // Vector has three float attributes.
	Vec3f operator+(const Vec3f &r) const {return Vec3f(x+r.x, y+r.y, z+r.z);} //Vector add
	Vec3f operator-(const Vec3f &r) const {return Vec3f(x-r.x, y-r.y, z-r.z);} //Vector sub
	Vec3f operator*(const Vec3f &r) const {return Vec3f(x*r.x, y*r.y, z*r.z);} //Multiplication terme à terme
	Vec3f operator*(float r) const {return Vec3f(x*r,y*r,z*r);} //Echelle
	auto operator % (const Vec3f &r) const ->decltype(x)
	{
		return x * r.x + y * r.y + z * r.z;
	}    //Vector dot product
	Vec3f operator^(const Vec3f &r) const {return Vec3f(y*r.z-z*r.y,z*r.x-x*r.z,x*r.y-y*r.x);} //Cross-product
	Vec3f operator-() const {return Vec3f(-x, -y, -z);} //Vector sub
    Vec3f(float a = 0., float b = 0., float c = 0.);
	Vec3f norm() const{return *this*(1 /sqrt(*this%*this));} // Normalise le vecteur
    Vec3f borne(const Vec3f& maxP);
};

#endif //POINT_3D_H
