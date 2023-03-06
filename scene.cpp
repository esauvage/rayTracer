#include "scene.h"

using namespace std;

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
	clear();
	deleteElems<Light>(lights);
}

Scene::Scene()
{

}

Scene::Scene(std::shared_ptr<Shape> shape)
{
	add(shape);
}

void Scene::clear()
{
	_world.clear();
}

void Scene::add(std::shared_ptr<Shape> object)
{
	_world.add(object);
}

std::map<std::string, std::shared_ptr<Material> > &Scene::materials()
{
	return _materials;
}

std::map<std::string, std::shared_ptr<Material> > Scene::materials() const
{
	return _materials;
}

Camera Scene::camera() const
{
	return _camera;
}

void Scene::setCamera(const Camera &newCamera)
{
	_camera = newCamera;
}
