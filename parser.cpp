#include "parser.h"

#include <iostream>
#include <map>
#include <exception>
#include <algorithm>
#include <memory>

#include "tokenStream.h"
#include "primitive_factory.h"
#include "lambertien.h"

using namespace std;
using namespace Eigen;

static shared_ptr<Lambertien> m[3] {make_shared<Lambertien>(Vec3f{0.95, 0.95, 0.95}),
			make_shared<Lambertien>(Vec3f{0.95, 0, 0.95}),
			make_shared<Lambertien>(Vec3f{0.95, 0.95, 0})};

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
			parameters["nom"] = name;
			scene.materials()[name] = MaterialFactory::create(parameters);
//			scene.materials[name] = make_shared<Lambertien>(Vec3f{parameters["red"].number(), parameters["green"].number(), parameters["blue"].number()});
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
			Camera c = scene.camera();
			c.setPosition(Vec3f(parameters["x"].number(), parameters["y"].number(), parameters["z"].number()));
			scene.setCamera(c);
			scene.cameraPos = {parameters["x"].number(), parameters["y"].number(), parameters["z"].number()};
			continue;
		}
		if (primitive == "cameraRot")
		{
			auto parameters = params();
			Camera c = scene.camera();
			c.setRotation(AngleAxis<float>(parameters["roll"].number(), Vector3f::UnitY())
					* AngleAxis<float>(parameters["yaw"].number(), Vector3f::UnitZ())
					* AngleAxis<float>(parameters["pitch"].number(), Vector3f::UnitX()));
			scene.setCamera(c);
			scene.cameraRot = AngleAxis<float>(parameters["roll"].number(), Vector3f::UnitY())
					* AngleAxis<float>(parameters["yaw"].number(), Vector3f::UnitZ())
					* AngleAxis<float>(parameters["pitch"].number(), Vector3f::UnitX());
			continue;
		}
		auto shapeParam = params();
		auto s = PrimitiveFactory::create(primitive, shapeParam);
		if (shapeParam.find("material") != shapeParam.end())
			s->setMaterial(scene.materials()[shapeParam["material"].text()]);
		scene.add(s);

		if (s->material() == nullptr)
			s->setMaterial(m[scene._world.objects().size()%3]);
	}
	return;
}

Parser::Parser(istream &is):_ts(is)
{

}

vector <shared_ptr<Shape> > Parser::prim(bool get)
{
	map <string, shared_ptr<Material> >matieres;
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
			matieres[name] = make_shared<Lambertien>(Vec3f{parameters["red"].number(), parameters["green"].number(), parameters["blue"].number()});
			continue;
		}
		auto shapeParam = params();
		shapes.push_back(PrimitiveFactory::create(primitive, shapeParam));
		if ((*shapes.rbegin())->material() == nullptr)
			(*shapes.rbegin())->setMaterial(m[shapes.size()%3]);
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
