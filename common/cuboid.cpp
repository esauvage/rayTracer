#include "cuboid.h"

#include "utils.h"
//#include <cmath>
using namespace std;
using namespace nlohmann;

#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

Cuboid::Cuboid(const Vec3f& min, const Vec3f& max)
    :_min(min), _max(max)
{

}

bool Cuboid::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
//char HitBoundingBox(minB,maxB, origin, dir,coord)
{
    Vec3f coord;				/* hit point */
    bool inside = true;
    Eigen::Vector3<char> quadrant;
    int i;
    int whichPlane;
    Vec3f maxT;
    Vec3f candidatePlane;

    /* Find candidate planes; this loop can be avoided if
    rays cast all from the eye(assume perpsective view) */
    for (i=0; i<3; i++)
    {
        if(r.origin()[i] < _min[i]) {
            quadrant[i] = LEFT;
            candidatePlane[i] = _min[i];
            inside = false;
            break;
        }else if (r.origin()[i] > _max[i]) {
            quadrant[i] = RIGHT;
            candidatePlane[i] = _max[i];
            inside = false;
            break;
        }else	{
            quadrant[i] = MIDDLE;
        }
    }

    /* Ray origin inside bounding box */
    if (inside)
    {
        coord = r.origin();
        return true;
    }

    /* Calculate T distances to candidate planes */
    for (i = 0; i < 3; i++)
        if (quadrant[i] != MIDDLE && r.direction()[i] != 0.)
            maxT[i] = (candidatePlane[i]-r.origin()[i]) / r.direction()[i];
        else
            maxT[i] = -1.;

    /* Get largest of the maxT's for final choice of intersection */
    whichPlane = 0;
    for (i = 1; i < 3; i++)
        if (maxT[whichPlane] < maxT[i])
            whichPlane = i;

    /* Check final candidate actually inside box */
    if (maxT[whichPlane] < 0.)
        return false;
    if ((maxT[whichPlane] < t_min) || (maxT[whichPlane] > t_max))
        return false;
    for (i = 0; i < 3; i++)
    {
        if (whichPlane != i)
        {
            coord[i] = r.origin()[i] + maxT[whichPlane] * r.direction()[i];
            if (coord[i] < _min[i] || coord[i] > _max[i])
                return false;
        } else
        {
            coord[i] = candidatePlane[i];
        }
    }
    auto d = r.distance(coord);
    rec.p = coord;
    rec.t = d;
    rec.pMaterial = material();
    Vec3f normal(0, 0, 0);
    normal[whichPlane] = 1;
    rec.setFaceNormal(r, normal);
    return true;				/* ray hits box */
}

// bool Cuboid::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
// {
//     HitRecord buf;
//     buf.t = INFINITY;
//     for (int a = 0; a < 3; a++) {
//         if (fabs(r.direction()[a])<1e-5)
//         {
//             if ((r.origin()[a] >= fmin(_min[a], _max[a])) && (r.origin()[a] <= fmax(_min[a], _max[a])))
//             {
//                 continue;
//             }
//             else
//                 return false;
//         }
//         auto invD = 1.0f / r.direction()[a];
//         auto t0 = (_min[a] - r.origin()[a]) * invD;
//         auto t1 = (_max[a] - r.origin()[a]) * invD;
//         if (invD < 0.0f)
//             std::swap(t0, t1);
//         t_min = t0 > t_min ? t0 : t_min;
//         t_max = t1 < t_max ? t1 : t_max;
//         if (t_max < t_min)
//             return false;
//         if (buf.t >= t_min)
//         {
//             buf.t = min(t_min, (float)rec.t);
//             buf.p = r.pointAt(rec.t);
//             Vec3f outwardNormal(0, 0, 0);
//             outwardNormal[a] = 1;
//             buf.setFaceNormal(r, outwardNormal);
//         }
//     }
//     rec = buf;
// //    rec.setTex(UV(rec.p));
//     rec.pMaterial = material();
//     return true;
// }

json &Cuboid::jsonHelper(json &j) const
{
    j = Shape::jsonHelper(j);
    j["min"] = _min;
    j["max"] = _max;
    return j;
}

bool Cuboid::boundingBox(double time0, double time1, AABB &outputBox) const
{
    (void)time0;
    (void)time1;
    outputBox = AABB(_min, _max);
    return true;
}
