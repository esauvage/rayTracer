#ifndef IMAGE_H
#define IMAGE_H

#include "material.h"
#include <string>
#include <CImg.h>

class Image : public Material
{
public:
    Image(const std::string &filename);
    std::string fileName() const;
    void setFileName(const std::string &newFileName);
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& attenuation, Rayon3f& scattered) const override;
    virtual bool scatter(const Rayon3f& r_in, const HitRecord& rec, Vec3f& localAttenuation, std::vector<Rayon3f>& scattered,
                         Vec3f& attenuation) const override;
    virtual nlohmann::json &jsonHelper(nlohmann::json &j) const override;

private:
    std::string _file;
    cimg_library::CImg<unsigned char> _image;
};

inline void to_json(nlohmann::json& j, const Image& m) {
    j = nlohmann::json{{"albedo", m.albedo()}, {"filename", m.fileName()}};
}

#endif // IMAGE_H
