#include "phongbliss.h"

#include "utils.h"

using namespace Eigen;
using namespace std;

PhongBliss::PhongBliss(const Vec3f& a)
    : Material(a)
{

}

bool PhongBliss::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    (void)r_in;
    while(true)
    {
        Vec3f scatter_direction = rec.normal() + random_unit_vector();
        // Catch degenerate scatter direction
        auto degenere = 0;
        for (auto x : scatter_direction)
        {
            degenere += fabs(x) > 1e-5 ? 0 : 1;
        }
        if (degenere > 2)
        {
            scatter_direction = rec.normal();
        }
        scattered = Rayon3f(rec.p, scatter_direction.stableNormalized());
        if (!scattered.direction().hasNaN())
            break;
        cout << "Ca vient du Lambertien !" << endl;
    }
    attenuation = albedo();
    return true;
}

bool PhongBliss::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &localAttenuation, vector<Rayon3f> &vScattered, Vec3f &attenuation) const
{
    auto degenere = 0;
    for (auto x : attenuation)
    {
        degenere += fabs(x) > 1e-2 ? 0 : 1;
    }
    if (degenere > 2)
    {
        return false;
    }
    for (int i = 0; i < 4; ++i)
    {
        Rayon3f scattered;
        scatter(r_in, rec, localAttenuation, scattered);
        vScattered.push_back(scattered);
    }
    attenuation = attenuation.cwiseProduct(localAttenuation);
    return true;
}