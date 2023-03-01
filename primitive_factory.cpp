#include "primitive_factory.h"

#include "boule.h"
#include "horizon.h"
#include "triangle.h"
#include "metal.h"
#include "lambertien.h"

using namespace std;

shared_ptr<Shape> PrimitiveFactory::create(const string &primName, const map <string, Parameter> &params)
{
	if (primName == "sphere")
	{
		return make_shared<Boule>(Vec3f(params.at("x").number(), params.at("y").number(), params.at("z").number()),
						 params.at("radius").number());
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

shared_ptr<Material> MaterialFactory::create(const map <string, Parameter> &params)
{
	if (params.at("type").text() == "metal")
	{
		return make_shared<Metal>(Vec3f(params.at("red").number(), params.at("green").number(), params.at("blue").number()));
	}
	if (params.at("type").text() == "lambertien")
	{
		return make_shared<Lambertien>(Vec3f(params.at("red").number(), params.at("green").number(), params.at("blue").number()));
	}
	return nullptr;
}
