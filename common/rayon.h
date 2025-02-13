#ifndef RAYON_H
#define RAYON_H

#include <Eigen/Geometry>

template<typename Type, int Size>
using Vec = Eigen::Matrix<Type, Size, 1>;
template<typename Type, int Size>
using Rayon = Eigen::ParametrizedLine<Type, Size>;
using Rayon3f = Rayon<double, 3>;

#endif // RAYON_H
