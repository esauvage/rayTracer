#ifndef CAMERA_H
#define CAMERA_H

#include "rayon.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using Rayon3f = Rayon<float, 3>;
using Vec3f = Vec<float, 3>;

class Camera
{
public:
	Camera();

	Rayon3f ray(double u, double v) const;

	Vec3f position() const;
	void setPosition(const Vec3f &newPosition);
	Eigen::Quaternion<float> rotation() const;
	void setRotation(const Eigen::Quaternion<float> &newRotation);

	float focusDist() const;
	void setFocusDist(float newFocusDist);

	void setLensRadius(float newLensRadius);

private:
	void update();

	Vec3f _position;
	Eigen::Quaternion<float> _rotation;
	Vec3f _lower_left_corner;
	Vec3f _horizontal;
	Vec3f _vertical;
	Vec3f _u, _v, _w;
	float _focalLength;
	float _focusDist;
	float _aspectRatio;
	float _lensRadius;
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

inline void to_json(json& j, const Camera& camera)
{
	j = json{{"position", camera.position()}, {"orientation", camera.rotation()}, {"focus", camera.focusDist()}};
}

inline void from_json(const json& j, Camera& camera)
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
