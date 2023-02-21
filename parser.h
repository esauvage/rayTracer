#ifndef PARSER_H
#define PARSER_H

#include "tokenStream.h"
#include "parameter.h"

#include <map>
#include <vector>
#include "scene.h"

using namespace std;

class Shape;

class Parser
{
public:
	Parser (istream &is):_ts(is){};
	vector <Shape *> prim(bool get);
	void readScene(Scene & scene);
private:
	void error(const string& s);

	Token_stream _ts;
	map <string, Parameter> params();
	int _nbErrors {0};
};

#endif //PARSER_H