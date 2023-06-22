#include "obj2json.h"

#include "objload.h"
#include "mesh.h"

#include <iostream>

using namespace std;
using namespace obj;

OBJ2JSON::OBJ2JSON()
{

}

void OBJ2JSON::convert(const string &in, const string &out)
{
	try {
		Model objMesh = loadModelFromFile(in);
		Mesh jsonMesh;
		for (size_t i = 0; i < objMesh.vertex.size(); i+=3)
		{
			jsonMesh.addVertex(Vec3f(objMesh.vertex[i], objMesh.vertex[i + 1], objMesh.vertex[i + 2]));
		}
		cout << "vertices : " << objMesh.vertex.size() << endl;
		cout << "UVs : " << objMesh.texCoord.size() << endl;
		for (size_t i = 0; i < objMesh.texCoord.size(); i+=2)
		{
			jsonMesh.addTex(Vec2f(objMesh.texCoord[i], objMesh.texCoord[i + 1]));
		}
		for (size_t i = 0; i < objMesh.normal.size(); i+=3)
		{
			jsonMesh.addNormal(Vec3f(objMesh.normal[i], objMesh.normal[i + 1], objMesh.normal[i + 2]));
		}
		for (const auto &f : objMesh.faces)
		{
			for (size_t i = 0; i < f.second.size(); i+=3)
			{
				Eigen::Vector3i ind(f.second[i], f.second[i+1], f.second[i+2]);
				jsonMesh.addVertI(ind);
			}
		}
		ofstream o(out);
		nlohmann::json j = jsonMesh;
		o << setw(4) << j << endl;
		o.close();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
