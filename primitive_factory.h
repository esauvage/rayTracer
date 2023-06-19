#ifndef PRIMITIVE_FACTORY
#define PRIMITIVE_FACTORY

#include "parameter.h"

#include <string>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
class Shape;
class Material;

class PrimitiveFactory
{
public:
    static std::shared_ptr<Shape> create(const std::string& primName, const std::map< std::string, Parameter >& params);
    static std::shared_ptr<Shape> from_json(const nlohmann::json & j,
                                            const std::map<std::string, std::shared_ptr<Material> > &materials);
};

class MaterialFactory
{
public:
    static std::shared_ptr<Material> create(const std::map< std::string, Parameter >& params);
    static std::shared_ptr<Material> from_json(const nlohmann::json & j, std::map<std::string, std::shared_ptr<Material> > &materials);
};
#endif //PRIMITIVE_FACTORY
