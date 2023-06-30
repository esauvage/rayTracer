#include "scene.h"

#include "phongbliss.h"
#include "lambertien.h"
#include "composedmaterial.h"

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
}

Scene::Scene()
{
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

void Scene::addSun(const Sun &sun)
{
	_suns.push_back(sun);
}

const std::vector<Sun> Scene::suns() const
{
    return _suns;
}

void Scene::updateMaterials()
{
    for (auto m:_materials)
    {
        std::shared_ptr<PhongBliss> t = dynamic_pointer_cast<PhongBliss>(m.second);
        if (t)
        {
            t->setScene(this);
        }
        std::shared_ptr<Lambertien> l = dynamic_pointer_cast<Lambertien>(m.second);
        if (l)
        {
            l->setScene(this);
        }
		std::shared_ptr<ComposedMaterial> c = dynamic_pointer_cast<ComposedMaterial>(m.second);
		if (c)
		{
			c->setScene(this);
		}
	}
}

Camera Scene::camera() const
{
	return _camera;
}

void Scene::setCamera(const Camera &newCamera)
{
	_camera = newCamera;
}
