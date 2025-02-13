#include "composedmaterial.h"

#include "utils.h"
#include "scene.h"
#include "phongbliss.h"
#include "lambertien.h"

using namespace Eigen;
using namespace std;
using namespace nlohmann;

ComposedMaterial::ComposedMaterial(const Vec3f& a)
 : Material(a)
{

}

bool ComposedMaterial::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
	bool r = false;
	attenuation = Vec3f(0, 0, 0);
	for (const auto &m : _materials)
	{
		Vec3f localAttenuation(0, 0, 0);
		r |= m->scatter(r_in, rec, localAttenuation, scattered);
		attenuation += localAttenuation;
	}
	attenuation/=_materials.size();
	return r;
}

json &ComposedMaterial::jsonHelper(json &j) const
{
	j = Material::jsonHelper(j);
	j["materials"] = json::array();
	for (const auto & m : materials())
	{
		j["materials"].push_back(m->jsonHelper(j));
	}
	return j;
}

void ComposedMaterial::addMaterial(std::shared_ptr<Material> material)
{
	_materials.push_back(material);
}

std::vector<std::shared_ptr<Material> > ComposedMaterial::materials() const
{
	return _materials;
}

void ComposedMaterial::setScene(Scene * scene)
{
	for (auto m:_materials)
	{
		std::shared_ptr<PhongBliss> t = dynamic_pointer_cast<PhongBliss>(m);
		if (t)
		{
			t->setScene(scene);
		}
		std::shared_ptr<ComposedMaterial> c = dynamic_pointer_cast<ComposedMaterial>(m);
		if (c)
		{
			c->setScene(scene);
		}
	}
}
