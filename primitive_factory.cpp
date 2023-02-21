#include "primitive_factory.h"

#include "boule.h"
#include "horizon.h"
#include "triangle.h"

Shape * PrimitiveFactory::create(const string &primName, const map <string, Parameter> &params)
{
	if (primName == "sphere")
	{
		return new Boule({params.at("x").number(), params.at("y").number(), params.at("z").number()},
						 params.at("radius").number());
	}
	if (primName == "horizon")
	{
		return new Horizon(params.at("elevation").number());
	}
	if (primName == "triangle")
	{
		return new Triangle({Vec3f{params.at("x0").number(), params.at("y0").number(), params.at("z0").number()},
			Vec3f(params.at("x1").number(), params.at("y1").number(), params.at("z1").number()), 
			Vec3f(params.at("x2").number(), params.at("y2").number(), params.at("z2").number())});
	}
	return nullptr;
}
