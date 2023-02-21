#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <string>

#include "shape.h"
#include "light.h"
#include "quaternion.h"

using namespace std;

class Scene
{
public:
	~Scene();
	vector <Shape *> shapes;
	vector <Light *> lights;
	map <string, Material>materials;
	Vec3f cameraPos{0, 0, 1};
	Quaternion<float> cameraRot{1, 0, 0, 0};
};

#endif
