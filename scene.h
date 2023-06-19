#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <map>
#include <string>
#include "shape.h"
#include "light.h"
#include "camera.h"

#include "primitive_factory.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector <Light *> lights;
	Camera camera() const;
	void setCamera(const Camera &newCamera);
	std::map<std::string, std::shared_ptr<Material> > &materials();
	std::map<std::string, std::shared_ptr<Material> > materials() const;

    const std::shared_ptr<Shape> &world() const;
    void setWorld(const std::shared_ptr<Shape> &newWorld);

private:
	Camera _camera;
	std::map <std::string, std::shared_ptr<Material> >_materials;
    std::shared_ptr<Shape> _world;
};

inline void to_json(nlohmann::json& j, const Scene& scene)
{
    j["camera"] = nlohmann::json(scene.camera());
    j["materials"] = nlohmann::json(scene.materials());
    j["world"] = nlohmann::json(scene.world());
}

inline void from_json(const nlohmann::json& j, Scene& scene)
{
	Camera camera;
	j.at("camera").get_to(camera);
	scene.setCamera(camera);
	for (const auto &m : j.at("materials"))
	{
        std::shared_ptr<Material> mat = MaterialFactory::from_json(m, scene.materials());
		scene.materials()[mat->nom()] = mat;
	}
    scene.setWorld(PrimitiveFactory::from_json(j.at("world"), scene.materials()));
}

#endif
