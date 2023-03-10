#ifndef RAYON_H
#define RAYON_H

#include <Eigen/Geometry>
//#include <memory>

template<typename Type, int Size>
using Vec = Eigen::Matrix<Type, Size, 1>;
template<typename Type, int Size>
using Rayon = Eigen::ParametrizedLine<Type, Size>;
//class Shape;

//template <typename Type, int Size>
//class Rayon
//{
//public:
//	Rayon()
//	{

//	}
//	Rayon(const Vec<Type, Size> & origin, const Vec<Type, Size> & direction, const std::shared_ptr<Shape> milieu = nullptr);

//	Vec<Type, Size> at(double t) const;
//	Vec<Type, Size> origin() const;
//	Vec<Type, Size> direction() const;

//	const std::shared_ptr<Shape> milieu() const;

//private:
//	Vec<Type, Size> _origin;
//	Vec<Type, Size> _direction;
//	std::shared_ptr<Shape> _milieu; //indice de réfraction du milieu courant (ie vitesse de la lumière)
//};

//template<typename Type, int Size>
//std::basic_ostream<char> &operator << (std::basic_ostream<char> & out, const Rayon<Type, Size>r)
//{
//	out << "Origin : " << r.origin() << " Direction " << r.direction();
//	return out;
//}

//template<typename Type, int Size>
//Rayon<Type, Size>::Rayon(const Vec<Type, Size> &origin, const Vec<Type, Size> &direction, const std::shared_ptr<Shape>milieu)
//	:_origin{origin}, _direction{direction}, _milieu{milieu}
//{

//}

//template<typename Type, int Size>
//Vec<Type, Size> Rayon<Type, Size>::at(double t) const
//{
//	return _origin + t*_direction;
//}

//template<typename Type, int Size>
//Vec<Type, Size> Rayon<Type, Size>::origin() const
//{
//	return _origin;
//}

//template<typename Type, int Size>
//Vec<Type, Size> Rayon<Type, Size>::direction() const
//{
//	return _direction;
//}

//template<typename Type, int Size>
//const std::shared_ptr<Shape> Rayon<Type, Size>::milieu() const
//{
//	return _milieu;
//}

#endif // RAYON_H
