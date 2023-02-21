#ifndef PRIMITIVE_FACTORY
#define PRIMITIVE_FACTORY

#include "parameter.h"

#include <string>
#include <map>

using namespace std;

class Shape;

class PrimitiveFactory
{
public:
	static Shape * create(const string& primName, const map< string, Parameter >& params);
};
#endif //PRIMITIVE_FACTORY