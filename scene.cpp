#include "scene.h"

#include "lambertien.h"
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

const std::shared_ptr<Shape> &Scene::world() const
{
    return _world;
}

void Scene::setWorld(const std::shared_ptr<Shape> &newWorld)
{
    _world = newWorld;
}

Camera Scene::camera() const
{
	return _camera;
}

void Scene::setCamera(const Camera &newCamera)
{
	_camera = newCamera;
}
