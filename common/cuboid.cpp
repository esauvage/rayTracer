#include "cuboid.h"

#include "utils.h"
//#include <cmath>
using namespace std;
using namespace nlohmann;

Cuboid::Cuboid(const Vec3f& min, const Vec3f& max)
    :_min(min), _max(max)
{

}

bool Cuboid::touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const
{
    HitRecord buf;
    buf.t = INFINITY;
    for (int a = 0; a < 3; a++) {
        if (fabs(r.direction()[a])<1e-5)
        {
            if ((r.origin()[a] >= fmin(_min[a], _max[a])) && (r.origin()[a] <= fmax(_min[a], _max[a])))
            {
                continue;
            }
            else
                return false;
        }
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (_min[a] - r.origin()[a]) * invD;
        auto t1 = (_max[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max < t_min)
            return false;
        if (buf.t >= t_min)
        {
            buf.t = min(t_min, (float)rec.t);
            buf.p = r.pointAt(rec.t);
            Vec3f outwardNormal(0, 0, 0);
            outwardNormal[a] = 1;
            buf.setFaceNormal(r, outwardNormal);
        }
    }
    rec = buf;
//    rec.setTex(UV(rec.p));
    rec.pMaterial = material();
    return true;
}

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
