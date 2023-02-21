#ifndef MAT_3D_H
#define MAT_3D_H

#include <math.h>

class Mat3D
{
public:
	float [9];  // Vector has three float attributes.
	Point3D operator+(const Point3D &r) const {return Point3D(x+r.x, y+r.y, z+r.z);} //Vector add
	Point3D operator-(const Point3D &r) const {return Point3D(x-r.x, y-r.y, z-r.z);} //Vector sub
	Point3D operator*(const Point3D &r) const {return Point3D(x*r.x, y*r.y, z*r.z);} //Multiplication terme à terme
	Point3D operator*(float r) const {return Point3D(x*r,y*r,z*r);} //Echelle
	auto operator % (const Point3D &r) const ->decltype(x)
	{
		return x * r.x + y * r.y + z * r.z;
	}    //Vector dot product
	Point3D operator^(const Point3D &r) const {return Point3D(y*r.z-z*r.y,z*r.x-x*r.z,x*r.y-y*r.x);} //Cross-product
    Point3D(float a = 0., float b = 0., float c = 0.);
//	Point3D norm(){return *this*(1 /sqrt(*this%*this));} // Normalise le vecteur
	Point3D norm() const{return *this*(1 /sqrt(*this%*this));} // Normalise le vecteur
    Point3D borne(const Point3D& maxP);
};

#endif //MAT_3D_H
