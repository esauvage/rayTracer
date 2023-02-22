#ifndef PRIMITIVE_FACTORY
#define PRIMITIVE_FACTORY

#include "parameter.h"

#include <string>
#include <map>
#include <memory>

class Shape;

class PrimitiveFactory
{
public:
	static std::shared_ptr<Shape> create(const string& primName, const map< string, Parameter >& params);
};
#endif //PRIMITIVE_FACTORY
