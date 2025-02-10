#ifndef CAMERA_H
#define CAMERA_H

#include "rayon.h"
#include <nlohmann/json.hpp>
#include <QVector3D>
#include <QMatrix4x4>

using Rayon3f = Rayon<float, 3>;
using Vec3f = Vec<float, 3>;

class Camera
{
public:
    Camera();
    Camera(const QVector3D &pos);

	Rayon3f ray(double u, double v) const;

	Vec3f position() const;
	void setPosition(const Vec3f &newPosition);
	Eigen::Quaternion<float> rotation() const;
	void setRotation(const Eigen::Quaternion<float> &newRotation);

	float focusDist() const;
	void setFocusDist(float newFocusDist);

	void setLensRadius(float newLensRadius);

    void yaw(float degrees);
    void pitch(float degrees);
    void walk(float amount);
    void strafe(float amount);

    QMatrix4x4 viewMatrix() const;

    void setSize(const QSize &newSize);

    QSize size() const;

private:
	void update();
    Vec3f defocusDiskSample() const;

	Vec3f _position;
	Eigen::Quaternion<float> _rotation;
	Vec3f _lower_left_corner;
	Vec3f _horizontal;
	Vec3f _vertical;
	Vec3f _u, _v, _w;
	float _focalLength;
	float _focusDist;
    float _defocusAngle;
	float _aspectRatio;
	float _lensRadius;
    Vec3f _defocusDiskU;       // Defocus disk horizontal radius
    Vec3f _defocusDiskV;       // Defocus disk vertical radius
    Vec3f _pixel00Loc;          // Location of pixel 0, 0
    Vec3f _pixelDeltaU;        // Offset to pixel to the right
    Vec3f _pixelDeltaV;        // Offset to pixel below


    QVector3D m_forward;
    QVector3D m_right;
    QVector3D m_up;
    float m_yaw;
    float m_pitch;
    QMatrix4x4 m_yawMatrix;
    QMatrix4x4 m_pitchMatrix;
    QSize _size;
};

namespace nlohmann {
	template <typename T>
	struct adl_serializer<Eigen::Quaternion<T>> {
		static void to_json(json& j, const Eigen::Quaternion<T>& q) {
			j = json{q.w(), q.x(), q.y(), q.z()};
		}

		static void from_json(const json& j, Eigen::Quaternion<T>& q) {
			q = Eigen::Quaternion<T>(j[0].get<T>(), j[1].get<T>(), j[2].get<T>(), j[3].get<T>());
		}
	};
}

inline void to_json(nlohmann::json& j, const Camera& camera)
{
    j = nlohmann::json{{"position", camera.position()}, {"orientation", camera.rotation()}, {"focus", camera.focusDist()}};
}

inline void from_json(const nlohmann::json& j, Camera& camera)
{
    Vec3f position = Vec3f(j.at("position")[0].get<float>(), j.at("position")[1].get<float>(), j.at("position")[2].get<float>());
//	j.at("position").get_to(position);
	camera.setPosition(position);
	Eigen::Quaternionf rotation;
	j.at("orientation").get_to(rotation);
    camera.setRotation(rotation);
	if (j.contains("lens radius"))
	{
		camera.setLensRadius(j.at("lens radius").get<float>());
	}
	if (j.contains("focus"))
	{
        camera.setFocusDist(j.at("focus").get<float>());
    }
}

#endif // CAMERA_H
