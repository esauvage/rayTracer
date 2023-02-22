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
//	deleteElems<Shape>(shapes);
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
	shapes.clear();
}

void Scene::add(std::shared_ptr<Shape> object)
{
	shapes.push_back(object);
}

bool Scene::touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& shape : shapes) {
		if (shape->touche(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

float Scene::distance(const Rayon3f & ray, float minDist) const
{
	(void)minDist;
	auto closest_so_far = INFINITY;

	for (const auto& shape : shapes) {
		const auto d = shape->distance(ray, minDist);
		if (d < closest_so_far)
		{
			closest_so_far = d;
		}
	}
	return closest_so_far;
}

float Scene::distanceMin(const Vec3f &origin) const
{
	auto closest_so_far = INFINITY;

	for (const auto& shape : shapes) {
		const auto d = shape->distanceMin(origin);
		if (d < closest_so_far)
		{
			closest_so_far = d;
		}
	}
	return closest_so_far;
}

Vec3f Scene::normal(const Vec3f &p) const
{
	Vec3f n;
	auto closest_so_far = INFINITY;

	for (const auto& shape : shapes) {
		const auto d = shape->distanceMin(p);
		if (d < closest_so_far)
		{
			closest_so_far = d;
			n = shape->normal(p);
		}
	}
	return n;
}
