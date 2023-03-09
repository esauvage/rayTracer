#ifndef PRIMITIVE_FACTORY
#define PRIMITIVE_FACTORY

#include "parameter.h"

#include <string>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Shape;
class Material;

class PrimitiveFactory
{
public:
	static std::shared_ptr<Shape> create(const string& primName, const map< string, Parameter >& params);
};

class MaterialFactory
{
public:
	static std::shared_ptr<Material> create(const map< string, Parameter >& params);
	static std::shared_ptr<Material> from_json(const json & j);
};
#endif //PRIMITIVE_FACTORY
