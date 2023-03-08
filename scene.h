#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <list>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "shapelist.h"
#include "light.h"
#include "camera.h"

class Scene
{
public:
	Scene();
	Scene(std::shared_ptr<Shape> object);
	~Scene();
	void clear();
	void add(std::shared_ptr<Shape> object);

	ShapeList _world;
	std::vector <Light *> lights;
	Vec3f cameraPos{0, 0, 1};
	Eigen::Quaternion<float> cameraRot{0, 0, 0, 1};
	Camera camera() const;
	void setCamera(const Camera &newCamera);
	std::map<std::string, std::shared_ptr<Material> > &materials();
	std::map<std::string, std::shared_ptr<Material> > materials() const;

private:
	Camera _camera;
	std::map <std::string, std::shared_ptr<Material> >_materials;
};

inline void to_json(json& j, const Scene& scene) {
	j = json{{"camera", scene.camera()}, {"materials", scene.materials()}, {"world", scene._world}};
}

#endif
