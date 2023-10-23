#include "camera.h"

#include "utils.h"

using namespace Eigen;

Camera::Camera()
    :_position(0, 0, 0), _rotation(0, 0, 0, 1), _focalLength(1.), _focusDist(6.5), _aspectRatio(4./3.),
    m_forward(0.0f, 0.0f, -1.0f),
    m_right(1.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_yaw(0.0f),
    m_pitch(0.0f)
{
    _lensRadius = 0.1 / 2.;
    update();
}

Camera::Camera(const QVector3D &pos)
    :_position(pos.x(), pos.y(), pos.z()), _rotation(0, 0, 0, 1), _focalLength(1.), _focusDist(6.5), _aspectRatio(4./3.),
    m_forward(0.0f, 0.0f, -1.0f),
    m_right(1.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_yaw(0.0f),
    m_pitch(0.0f)
{
    _lensRadius = 0.1 / 2.;
    update();
}

Rayon3f Camera::ray(double u, double v) const
{
	Vec3f rd = _lensRadius * random_in_unit_disk();
    Vec3f offset = _u * rd.x() + _v * rd.y();
    Vec3f screenCoord = _lower_left_corner + u*_horizontal + v*_vertical;
    Vec3f direction = (screenCoord - _position - offset).normalized();
    return Rayon3f(
        _position + offset,
        direction);
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
//	float theta = M_PI/3.;
//	float h = tan(theta/2.);
//	float viewport_height = 2.0 * h;
//	float viewport_width = _aspectRatio * viewport_height;

//	Vec3f vUp = _rotation * Vec3f(0, 1, 0);
    _w = _rotation * Vec3f(0, 0, 1);
    _u = Vec3f(m_right.x(), m_right.y(), m_right.z());//vUp.cross(_w).normalized();//unit_vector(cross(vup, w));
    _v = Vec3f(m_up.x(), m_up.y(), m_up.z());//w.cross(_u);

//	_horizontal = _focusDist * viewport_width * _u;
    //    _vertical = _focusDist * viewport_height * _v;
    _horizontal = _focusDist * _size.width() /(float)_size.height() * _u;
    _vertical = _focusDist * _v;
//    _lower_left_corner = _position - _horizontal/2. - _vertical/2. - _focusDist * m_forward;
    _lower_left_corner = Vec3f(-_focusDist * _size.width() /(float)_size.height()/2., -_focusDist/2., -_focalLength);
    auto buf = (QVector4D(_lower_left_corner.x(), _lower_left_corner.y(), _lower_left_corner.z(), 0.0f) *viewMatrix()).toVector3D();
    _lower_left_corner = Vec3f(buf.x(), buf.y(), buf.z());
    _lower_left_corner+= _position;
}

QSize Camera::size() const
{
    return _size;
}

void Camera::setSize(const QSize &newSize)
{
    _size = newSize;
    update();
}

void Camera::setLensRadius(float newLensRadius)
{
	_lensRadius = newLensRadius;
}

float Camera::focusDist() const
{
	return _focusDist;
}

void Camera::setFocusDist(float newFocusDist)
{
	_focusDist = newFocusDist;
	update();
}

static inline void clamp360(float *v)
{
    if (*v > 180.0f)
        *v -= 360.0f;
    if (*v < -180.0f)
        *v += 360.0f;
}

void Camera::yaw(float degrees)
{
    m_yaw += degrees;
    clamp360(&m_yaw);
    m_yawMatrix.setToIdentity();
    m_yawMatrix.rotate(m_yaw, 0, 1, 0);

    QMatrix4x4 rotMat = m_pitchMatrix * m_yawMatrix;
    m_forward = (QVector4D(0.0f, 0.0f, -1.0f, 0.0f) * rotMat).toVector3D();
    m_right = (QVector4D(1.0f, 0.0f, 0.0f, 0.0f) * rotMat).toVector3D();
    update();
}

void Camera::pitch(float degrees)
{
    m_pitch += degrees;
    clamp360(&m_pitch);
    m_pitchMatrix.setToIdentity();
    m_pitchMatrix.rotate(m_pitch, 1, 0, 0);

    QMatrix4x4 rotMat = m_pitchMatrix * m_yawMatrix;
    m_forward = (QVector4D(0.0f, 0.0f, -1.0f, 0.0f) * rotMat).toVector3D();
    m_up = (QVector4D(0.0f, 1.0f, 0.0f, 0.0f) * rotMat).toVector3D();
    update();
}

void Camera::walk(float amount)
{
    _position += amount * Vec3f(m_forward[0],m_forward[1], m_forward[2]);
    update();
}

void Camera::strafe(float amount)
{
    _position[0] += amount * m_right.x();
    _position[2] += amount * m_right.z();
    update();
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 m = m_pitchMatrix * m_yawMatrix;
    m.translate(-QVector3D(_position[0], _position[1], _position[2]));
    return m;
}
