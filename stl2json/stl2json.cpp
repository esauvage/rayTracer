#include "stl2json.h"

#include "stl_reader.h"
#include "mesh.h"

#include <iostream>

using namespace std;

STL2JSON::STL2JSON()
{

}

void STL2JSON::convert(const string &in, const string &out)
{
    try {
        stl_reader::StlMesh <float, unsigned int> mesh (in);
        Mesh jsonMesh;
        for (size_t iVert = 0; iVert < mesh.num_vrts(); ++iVert)
        {
            jsonMesh.add(Vec3f(mesh.vrt_coords(iVert)));
        }
        for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
            Eigen::Vector3i ind(mesh.tri_corner_ind(itri, 0), mesh.tri_corner_ind(itri, 1), mesh.tri_corner_ind(itri, 2));
            jsonMesh.add(ind);

//            shared_ptr <Triangle> jsonTriangle = make_shared<Triangle>(Vec3f(mesh.tri_corner_coords (itri, 0)),
//                                Vec3f(mesh.tri_corner_coords (itri, 1)),
//                                Vec3f(mesh.tri_corner_coords (itri, 2)));
//            jsonMesh.add(jsonTriangle);
//            std::cout << "coordinates of triangle " << itri << ": ";
//            for(size_t icorner = 0; icorner < 3; ++icorner) {
//                const float* c = mesh.tri_corner_coords (itri, icorner);
//                // or alternatively:
//                // float* c = mesh.vrt_coords (mesh.tri_corner_ind (itri, icorner));
//                std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
//            }
//            std::cout << std::endl;

//            const float* n = mesh.tri_normal(itri);
//            std::cout << "normal of triangle " << itri << ": "
//                      << "(" << n[0] << ", " << n[1] << ", " << n[2] << ")\n";
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
