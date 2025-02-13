#include "lambertien.h"

#include "utils.h"

using namespace Eigen;

Lambertien::Lambertien(const Vec3f& a)
 : Material(a)
{

}

bool Lambertien::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    (void)r_in;
    // if (!_scene)
    //     return false;
    attenuation = albedo();
    Vec3f scatter_direction = rec.normal() + random_unit_vector<double>();

        //Intensity of the specular light
    //     float NdotL = fmax(0, rec.normal().dot(sun.direction()));
    //     float intensity = NdotL;
    //     if (intensity != intensity) continue;
    //     //Sum up the specular light factoring
    //     attenuation += albedo().cwiseProduct(sun.color()) * intensity;
    // }
    // scatter_direction /= (float)nbSuns;
    // Catch degenerate scatter direction
    if (scatter_direction.squaredNorm() < 0.01)
        scatter_direction = rec.normal();
    scattered = Rayon3f(rec.p, scatter_direction);
    return true;
}
