#include "primitive_factory.h"

#include "boule.h"
#include "horizon.h"
#include "triangle.h"
#include "shapelist.h"
#include "mesh.h"

#include "metal.h"
#include "lambertien.h"
#include "dielectrique.h"

using namespace std;
using namespace nlohmann;
using namespace Eigen;

shared_ptr<Shape> PrimitiveFactory::create(const string &primName, const map <string, Parameter> &params)
{
    if (primName == "sphere")
    {
        shared_ptr<Shape> r = make_shared<Boule>(Vec3f(params.at("x").number(), params.at("y").number(), params.at("z").number()),
                         params.at("radius").number());
        return r;
    }
    if (primName == "horizon")
    {
        return make_shared<Horizon>(params.at("elevation").number());
    }
    if (primName == "triangle")
    {
        return make_shared<Triangle>(array< Vec3f, 3>{Vec3f{params.at("x0").number(), params.at("y0").number(), params.at("z0").number()},
            Vec3f(params.at("x1").number(), params.at("y1").number(), params.at("z1").number()),
            Vec3f(params.at("x2").number(), params.at("y2").number(), params.at("z2").number())});
    }
    return nullptr;
}

shared_ptr<Shape> PrimitiveFactory::from_json(const json &j, const map<string, shared_ptr<Material> > &materials)
{
    if (j.is_array())
    {
        if (j.size() == 1)
        {
            return PrimitiveFactory::from_json(j[0], materials);
        }
        shared_ptr<ShapeList> r = make_shared<ShapeList>();
        for (const auto &s : j)
        {
            r->add(PrimitiveFactory::from_json(s, materials));
        }
        return r;
    }
    shared_ptr<Shape> r{nullptr};
    if (j.contains("rayon"))
    {
        r = make_shared<Boule>(Vec3f(j.at("position")[0].get<float>(), j.at("position")[1].get<float>(), j.at("position")[2].get<float>()),
                j["rayon"].get<float>());
    }
    if (j.contains("hauteur"))
    {
        r = make_shared<Horizon>(j.at("hauteur").get<float>());
    }
	if (j.contains("itriangles") || j.contains("triangles"))
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		if (j.contains("points"))
		{
			for (const auto &point : j["points"])
			{
				mesh->add(Vec3f(point[0].get<float>(), point[1].get<float>(), point[2].get<float>()));
			}
			if (j.contains("itriangles"))
			{
				for (const auto &i : j["itriangles"])
				{
					mesh->add(Vector3i(i[0].get<int>(), i[1].get<int>(), i[2].get<int>()));
				}
			}
		}
		if (j.contains("triangles"))
		{
			for (const auto &s : j["triangles"])
			{
				mesh->add(PrimitiveFactory::from_json(s, materials));
			}
		}
		mesh->update();
		r = mesh;
	}
	else if (j.contains("points"))
    {
        array< Vec3f, 3> a;
        int i = 0;
        for (auto m : j.at("points"))
        {
            a[i] = Vec3f(m[0].get<float>(), m[1].get<float>(), m[2].get<float>());
            i++;
        }
        r = make_shared<Triangle>(a);
    }
    if (j.contains("material") && (materials.find(j.at("material").get<string>()) != materials.end()))
    {
        r->setMaterial(materials.at(j.at("material").get<string>()));
    }
    return r;
}

shared_ptr<Material> MaterialFactory::create(const map <string, Parameter> &params)
{
	shared_ptr<Material> m;
	if (params.at("type").text() == "metal")
	{
		m = make_shared<Metal>(Vec3f(params.at("red").number(), params.at("green").number(), params.at("blue").number()),
								  params.at("fuzziness").number());
	}
	if (params.at("type").text() == "lambertien")
	{
		m = make_shared<Lambertien>(Vec3f(params.at("red").number(), params.at("green").number(), params.at("blue").number()));
	}
	if (params.at("type").text() == "dielectrique")
	{
		m = make_shared<Dielectrique>(params.at("refraction").number());
	}
	m->setNom(params.at("nom").text());
	return m;
}

std::shared_ptr<Material> MaterialFactory::from_json(const json &j)
{
	if (!j.contains("albedo"))
		return MaterialFactory::from_json(j[0]);
	shared_ptr<Material> m;
	if (j.contains("fuzz"))
	{
		m = make_shared<Metal>(Vec3f(j.at("albedo")[0].get<float>(), j.at("albedo")[1].get<float>(), j.at("albedo")[2].get<float>()),
								  j.at("fuzz").get<float>());
	}
	else if (j.contains("refractiveIndex"))
	{
		m = make_shared<Dielectrique>(j.at("refractiveIndex").get<float>());
	}
	else
	{
		m = make_shared<Lambertien>(Vec3f(j.at("albedo")[0].get<float>(), j.at("albedo")[1].get<float>(), j.at("albedo")[2].get<float>()));
	}
	m->setNom(j.at("nom").get<string>());
	return m;
}
