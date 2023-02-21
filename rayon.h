#ifndef RAYON_H
#define RAYON_H

//#include "shape.h"
#include <Eigen/Geometry>

template<typename Type, int Size>
using Vec  = Eigen::Matrix<Type, Size, 1>;
class Shape;
template <typename Type, int Size>class Rayon
{
public:
	Rayon(const Vec<Type, Size> & origin, const Vec<Type, Size> & direction, const Shape * milieu);

	Vec<Type, Size> at(double t) const;

	Vec<Type, Size> origin() const;

	Vec<Type, Size> direction() const;

	const Shape *milieu() const;

private:
	Vec<Type, Size> _origin;
	Vec<Type, Size> _direction;
	const Shape * _milieu; //indice de réfraction du milieu courant (ie vitesse de la lumière)
};
template<typename Type, int Size>
Rayon<Type, Size>::Rayon(const Vec<Type, Size> &origin, const Vec<Type, Size> &direction, const Shape *milieu)
	:_origin{origin}, _direction{direction}, _milieu{milieu}
{

}

template<typename Type, int Size>
Vec<Type, Size> Rayon<Type, Size>::at(double t) const
{
	return _origin + t*_direction;
}

template<typename Type, int Size>
Vec<Type, Size> Rayon<Type, Size>::origin() const
{
	return _origin;
}

template<typename Type, int Size>
Vec<Type, Size> Rayon<Type, Size>::direction() const
{
	return _direction;
}

template<typename Type, int Size>
const Shape *Rayon<Type, Size>::milieu() const
{
	return _milieu;
}

#endif // RAYON_H
