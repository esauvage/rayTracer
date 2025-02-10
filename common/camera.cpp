#include "camera.h"

#include "utils.h"

using namespace Eigen;

Camera::Camera()
    :_position(0, 0, 0), _rotation(0, 0, 0, 1), _focalLength(6.), _focusDist(6.5), _aspectRatio(4./3.),
    m_forward(0.0f, 0.0f, -1.0f),
    m_right(1.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_yaw(0.0f),
    m_pitch(0.0f),
    _defocusAngle(0)
{
    _lensRadius = 0.1 / 2.;
    update();
}

Camera::Camera(const QVector3D &pos)
    :_position(pos.x(), pos.y(), pos.z()), _rotation(0, 0, 0, 1), _focalLength(6.), _focusDist(6.5), _aspectRatio(4./3.),
    m_forward(0.0f, 0.0f, -1.0f),
    m_right(1.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_yaw(0.0f),
    m_pitch(0.0f),
    _defocusAngle(0)
{
    _lensRadius = 0.05 / 2.;
    update();
}

Rayon3f Camera::ray(double u, double v) const
{
    float theta = M_PI/4.;
    float h = tan(theta/2.);
    float w = h * _size.width() / _size.height();
//    Vec3f offset = _lensRadius * random_in_unit_disk();
//    offset[2] = 0;
    //On a la position du coin en bas à gauche de l'écran.
    //Il me faut la taille d'un pixel
 //   Vec3f offset = _u * rd.x() + _v * rd.y();
    auto offset = Vec3f(0, 0, 0);
    Vec3f pixel_sample = _pixel00Loc
                        + ((u-0.5 + offset.x()) * _pixelDeltaU)
                        + ((v-0.5 + offset.y()) * _pixelDeltaV);
//    pixel_sample[2] += _focusDist;
    auto ray_origin = (_defocusAngle <= 0) ? _position : defocusDiskSample();
    Vec3f buf = pixel_sample - ray_origin;

//    auto origin = _position + offset;
//    auto a = _lower_left_corner + u*_horizontal + v*_vertical;
//    auto buf = (QVector4D(a[0]*w, a[1]*h, a[2], 0.0f) *viewMatrix()).toVector3D();
//    buf -= QVector3D(origin[0], origin[1], origin[2]);
    auto buf2 = (QVector4D((u-0.5)*w, (v-0.5)*h, -1., 0.0f) *viewMatrix()).toVector3D();
    buf.normalize();
    buf2.normalize();
    Vec3f direction = (_rotation * Vec3f(buf.x(), buf.y(), buf.z())).normalized();
    //Attention, l'image est générée INVERSEE verticalement. C'est normal.
    return Rayon3f(
        ray_origin,
        direction);
}

Vec3f Camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return _position + (p[0] * _defocusDiskU) + (p[1] * _defocusDiskV);
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
//	float h = tan(theta/2.);
//	float viewport_height = 2.0 * h;
//	float viewport_width = _aspectRatio * viewport_height;

//	Vec3f vUp = _rotation * Vec3f(0, 1, 0);
    _w = _rotation * Vec3f(0, 0, 1);
    _u = Vec3f(m_right.x(), m_right.y(), m_right.z());//vUp.cross(_w).normalized();//unit_vector(cross(vup, w));
    _v = Vec3f(m_up.x(), m_up.y(), m_up.z());//w.cross(_u);

//	_horizontal = _focusDist * viewport_width * _u;
    //    _vertical = _focusDist * viewport_height * _v;
    _horizontal = _size.width() * _u;
    _vertical = _size.height() * _v;
//    _lower_left_corner = _horizontal/2. + _vertical/2. - _focusDist * Vec3f(m_forward.x(), m_forward.y(), m_forward.z());
    _lower_left_corner = Vec3f(- _size.width() /2., -_size.height() /2., -_focalLength);
    auto buf = (QVector4D(_lower_left_corner.x(), _lower_left_corner.y(), _lower_left_corner.z(), 0.0f) *viewMatrix()).toVector3D();
    _lower_left_corner = Vec3f(buf.x(), buf.y(), buf.z());
    _lower_left_corner += _position;



    // Determine viewport dimensions.
    float theta = M_PI/4.;
    auto h = tan(theta/2);
    auto viewport_height = 2 * h * _focusDist;
    auto viewport_width = viewport_height * _size.width()/_size.height();

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
//    w = unit_vector(lookfrom - lookat);
//    u = unit_vector(cross(vup, w));
//    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vec3f viewport_u = viewport_width * _u;    // Vector across viewport horizontal edge
    Vec3f viewport_v = viewport_height * -_v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors to the next pixel.
    _pixelDeltaU = viewport_u / _size.width();
    _pixelDeltaV = viewport_v / _size.height();

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = _position - Vec3f(0, 0, _focusDist) - viewport_u/2 - viewport_v/2;
    _pixel00Loc = viewport_upper_left - 0.5 * (_pixelDeltaU + _pixelDeltaV);

    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = _focusDist * tan(_defocusAngle / 2.);
    _defocusDiskU = _u * defocus_radius;
    _defocusDiskV = _v * defocus_radius;
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
