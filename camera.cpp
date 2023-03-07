#include "camera.h"

#include "utils.h"

using namespace Eigen;

Camera::Camera()
	:_position(0, 0, 0), _rotation(0, 0, 0, 1), _focalLength(1.), _focusDist(7), _aspectRatio(4./3.)
{
	_lensRadius = 0.5 / 2.;
	update();
}

Rayon3f Camera::ray(double u, double v) const
{
	Vec3f rd = _lensRadius * random_in_unit_disk();
	Vec3f offset = _u * rd.x() + _v * rd.y();

	return Rayon3f(
		_position + offset,
		(_lower_left_corner + u*_horizontal + v*_vertical - _position - offset));

//	return Rayon3f(_position, _rotation * (_lower_left_corner + u*_horizontal + v*_vertical - _position));
}

Vec3f Camera::position() const
{
	return _position;
}

void Camera::setPosition(const Vec3f &newPosition)
{
	_position = newPosition;
	update();
}

Quaternion<float> Camera::rotation() const
{
	return _rotation;
}

void Camera::setRotation(const Quaternion<float> &newRotation)
{
	_rotation = newRotation;
	update();
}

void Camera::update()
{
	auto theta = M_PI/3.;
	auto h = tan(theta/2.);
	auto viewport_height = 2.0 * h;
	auto viewport_width = _aspectRatio * viewport_height;

	Vec3f vUp = _rotation * Vec3f(0, 1, 0);
	_w = _rotation * Vec3f(0, 0, -1);
	_u = vUp.cross(_w).normalized();//unit_vector(cross(vup, w));
	_v = _w.cross(_u);

	_horizontal = _focusDist * viewport_width * _u;
	_vertical = _focusDist * viewport_height * _v;
	_lower_left_corner = _position - _horizontal/2. - _vertical/2. - _focusDist * _w;
}
