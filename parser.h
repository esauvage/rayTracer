#ifndef PARSER_H
#define PARSER_H

#include "tokenStream.h"
#include "parameter.h"

//#include <map>
//#include <vector>
#include "scene.h"

class Shape;

class Parser
{
public:
	Parser (std::istream &is);
	std::vector <std::shared_ptr<Shape> > prim(bool get);
	void readScene(Scene & scene);
private:
	void error(const std::string& s);

	Token_stream _ts;
	std::map <std::string, Parameter> params();
	int _nbErrors {0};
};

#endif //PARSER_H
