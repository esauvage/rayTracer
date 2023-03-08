#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "scene.h"
#include "CImg.h"
using namespace cimg_library;

using Rayon3f = Rayon<float, 3>;
using Vec3f = Vec<float, 3>;

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	void generateFile(const std::string& outFile, const std::pair <int, int> size, const std::string& inFile);
private:
	std::pair<float, std::shared_ptr<Shape>> nearestShape(const Rayon3f &rayon);
	Vec3f rayonRefracte(Vec3f normal, Vec3f incident, float n1, float n2);
	Vec3f pixelColor(const Rayon3f &rayon, int depth) const;
	Vec3f sky(const Vec3f& rayon) const;
	void fillImage(int rowBegin, int nbRows, CImg<unsigned char> &img) const;
	Scene scene;
//	std::vector <std::shared_ptr<Shape>>orderedShapes;
};

#endif // RAY_TRACER_H
