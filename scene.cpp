#include "scene.h"

template <typename T>
void deleteElems(vector <T *> scene)
{
	for (auto i:scene)
	{
		delete i;
	}
}

Scene::~Scene()
{
	deleteElems<Shape>(shapes);
	deleteElems<Light>(lights);
}
