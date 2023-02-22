#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "scene.h"
#include "rayon.h"

using Rayon3f = Rayon<float, 3>;
using Vec3f  = Vec<float, 3>;

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	void generateFile(const std::string& outFile, const std::pair <int, int> size, const std::string& inFile);
private:
	std::pair<float, std::shared_ptr<Shape>> nearestShape(const Rayon3f &rayon);
	Vec3f rayonRefracte(Vec3f normal, Vec3f incident, float n1, float n2);
	void distToShape(float *r, std::shared_ptr<Shape>s, const Rayon3f &rayon);
	Vec3f pixelColor(Rayon3f rayon);
	Vec3f sky(const Vec3f& rayon);
	void fillImage(std::ofstream &out, int rowBegin, int nbRows, int width, int height);
	Scene scene;
	std::vector <std::shared_ptr<Shape>>orderedShapes;
};

#endif // RAY_TRACER_H
