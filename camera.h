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

private:
	Vec3f _position;
	Eigen::Quaternion<float> _rotation;
	Vec3f _lower_left_corner;
	Vec3f _horizontal;
	Vec3f _vertical;
	Vec3f _u, _v, _w;
	float _aspectRatio;
	float _focalLength;
	float _lensRadius;
	float _focusDist;
	void update();
};

inline void to_json(json& j, const Vec3f& v)
{
	j = json{v.x(), v.y(), v.z()};
}

namespace nlohmann {
	template <typename T>
	struct adl_serializer<Eigen::Quaternion<T>> {
		static void to_json(json& j, const Eigen::Quaternion<T>& q) {
			j = json{q.x(), q.y(), q.z(), q.w()};
		}

		static void from_json(const json& j, Eigen::Quaternion<T>& q) {
				q = j.get<T>(); // same as above, but with
								  // adl_serializer<T>::from_json
			}
		};
}


inline void to_json(json& j, const Camera& camera)
{
	j = json{{"position", camera.position()}, {"orientation", camera.rotation()}};
}
#endif // CAMERA_H