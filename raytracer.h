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
	void generateFile(const string& outFile, const pair <int, int> size, const string& inFile);
private:
	pair<float, Shape *> nearestShape(const Rayon3f &rayon);
	Vec3f rayonRefracte(Vec3f normal, Vec3f incident, float n1, float n2);
	void distToShape(float *r, Shape *s, Vec3f origin, Vec3f direction);
	Vec3f pixelColor(Rayon3f rayon);
	Vec3f sky(const Vec3f& rayon);
	void fillImage(std::ofstream &out, int rowBegin, int nbRows, int width, int height);
	Scene scene;
	vector <Shape *>orderedShapes;
};

#endif // RAY_TRACER_H
