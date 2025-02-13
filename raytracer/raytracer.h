#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <atomic>

#include <CImg.h>
#include "../common/scene.h"

using Rayon3f = Rayon<double, 3>;
using Vec3f = Vec<double, 3>;

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();
    void generateFile(const std::string& outFile, const std::pair <int, int> size, const std::string& inFile);
private:
	Vec3f pixelColor(const Rayon3f &rayon, int depth, Vec3f &attenuation) const;
    Vec3f sky(const Vec3f& rayon) const;
	void fillImage(int rowBegin, int nbRows, cimg_library::CImg<unsigned char> *img) const;
    void updateDisplay(cimg_library::CImgDisplay * display, cimg_library::CImg<unsigned char> *img) const;

    Scene scene;
    std::atomic_int _activeThreads;
    bool _computing;
};

#endif // RAY_TRACER_H
