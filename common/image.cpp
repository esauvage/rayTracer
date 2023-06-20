#include "image.h"

#include "utils.h"
#include <iostream>

using namespace std;

Image::Image(const string &filename)
    : Material(Vec3f(1., 1., 1.)), _file(filename)
{
    setFileName(_file);
}

std::string Image::fileName() const
{
    return _file;
}

void Image::setFileName(const std::string &newFile)
{
    _file = newFile;
    _image = cimg_library::CImg<unsigned char>(_file.c_str());
}

bool Image::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &attenuation, Rayon3f &scattered) const
{
    (void)r_in;
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
    scattered = Rayon3f(rec.p, scatter_direction.normalized());
    const int u = (int)(rec.tex()[1] * _image.height());
    const int v = (int)(4*rec.tex()[0] * _image.width())%_image.width();
    attenuation = Vec3f(_image(v,u,0,0)/255., _image(v,u,0,1)/255., _image(v,u,0,2)/255.);
    return true;
}

nlohmann::json &Image::jsonHelper(nlohmann::json &j) const
{
    j = Material::jsonHelper(j);
    j["filename"] = _file;
    return j;
}

bool Image::scatter(const Rayon3f &r_in, const HitRecord &rec, Vec3f &localAttenuation, vector<Rayon3f> &vScattered, Vec3f &attenuation) const
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
