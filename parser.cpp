#include "parser.h"

#include <iostream>
#include <map>
#include <exception>
#include <algorithm>

#include "tokenStream.h"
#include "primitive_factory.h"

using namespace std;

static Material m[3] {{Vec3f{250, 250, 250}, 0.4, 1.75, 0.8}, {Vec3f{250, 250, 250}, 0.4, 1.5, 0.6},
				{Vec3f{250, 250, 0}, 0.4, 1.25, 0.4}};

void Parser::readScene(Scene & scene)
{
	while (true)
	{
		_ts.get();
		switch (_ts.current().kind)
		{
			case Kind::end :
			case Kind::group :
				return;// c'est un séparateur, tout va bien
			default:
				if (_ts.current().kind != Kind::type)
				{
					error("Name of a primitive expected");
					return;
				}
		}
		auto primitive = _ts.current().string_value;
		if (primitive == "material")
		{
			_ts.get();
			if (_ts.current().kind != Kind::type)
			{
				error("Name of a parameter expected for material");
				continue;
			}
			const string name = _ts.current().string_value;
			auto parameters = params();
			scene.materials[name] = {{parameters["red"].number(), parameters["green"].number(), parameters["blue"].number()},
				parameters["reflectance"].number(), 1, 0};
			continue;
		}
		if (primitive == "light")
		{
			auto parameters = params();
			scene.lights.push_back(new Light(Vec3f(parameters["x"].number(), parameters["y"].number(), parameters["z"].number()), 
				{parameters["red"].number(), parameters["green"].number(), parameters["blue"].number()}));
			continue;
		}
		if (primitive == "cameraPos")
		{
			auto parameters = params();
			scene.cameraPos = {parameters["x"].number(), parameters["y"].number(), parameters["z"].number()};
			continue;
		}
		if (primitive == "cameraRot")
		{
			auto parameters = params();
			scene.cameraRot = Quaternion<float>::fromEuler(parameters["pitch"].number(), parameters["roll"].number(), parameters["yaw"].number());
			continue;
		}
		auto shapeParam = params();
		auto s = PrimitiveFactory::create(primitive, shapeParam);
		if (shapeParam.find("material") != shapeParam.end())
			s->material = &scene.materials[shapeParam["material"].text()];
		scene.shapes.push_back(s);

		if (s->material == nullptr)
			s->material = &m[scene.shapes.size()%3];
	}
	return;
}

Parser::Parser(istream &is):_ts(is)
{

}

vector <shared_ptr<Shape> > Parser::prim(bool get)
{
	map <string, Material>matieres;
	vector <shared_ptr<Shape> > shapes;
	while (true)
	{
		if (get) 
			_ts.get();
		switch (_ts.current().kind)
		{
			case Kind::end :
			case Kind::group :
				return shapes;// c'est un séparateur, tout va bien
			default:
				if (_ts.current().kind != Kind::type)
				{
					error("Name of a primitive expected");
					return shapes;
				}
		}
		auto primitive = _ts.current().string_value;
		if (primitive == "material")
		{
			_ts.get();
			if (_ts.current().kind != Kind::type)
			{
				error("Name of a parameter expected");
				continue;//
			}
			const string name = _ts.current().string_value;
			auto parameters = params();
			matieres[name] = {{parameters["red"].number(), parameters["green"].number(), parameters["blue"].number()},
				parameters["reflectance"].number(), 1., 0.};
			continue;
		}
		auto shapeParam = params();
		shapes.push_back(PrimitiveFactory::create(primitive, shapeParam));
		if ((*shapes.rbegin())->material == nullptr)
			(*shapes.rbegin())->material = &m[shapes.size()%3];
	}
	return shapes;
}

map< string, Parameter > Parser::params()
{
	map< string, Parameter > p;
	while (true)
	{
		_ts.get();
		switch (_ts.current().kind)
		{
			case Kind::sep :
			case Kind::end :
			case Kind::group :
				return p;// c'est un séparateur, tout va bien
			default:
				if (_ts.current().kind != Kind::type)
				{
					error("Name of a parameter expected : got a number." + _ts.current().string_value);
					return p;
				}
		}
		string key = _ts.current().string_value;
		_ts.get();
		if (_ts.current().kind == Kind::number)
		{
			p.insert({key, _ts.current().number_value});
			continue;
		}
		if (_ts.current().kind == Kind::type)
		{
			p.insert({key, _ts.current().string_value});
			continue;
		}
		error("Value of a parameter expected");
		return p;//
	}
	return p;
}

void Parser::error(const string &s)
{
	_nbErrors++;
	cerr << "error : " << s << endl;
	throw std::runtime_error(s);
}
