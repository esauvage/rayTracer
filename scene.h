#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <list>

#include "shapelist.h"
#include "light.h"

class Scene// : public Shape
{
public:
	Scene();
	Scene(std::shared_ptr<Shape> object);
	~Scene();
	void clear();
	void add(std::shared_ptr<Shape> object);

	ShapeList _world;
	std::vector <Light *> lights;
	std::map <std::string, std::shared_ptr<Material> >materials;
	Vec3f cameraPos{0, 0, 1};
	Eigen::Quaternion<float> cameraRot{1, 0, 0, 0};
};

#endif
