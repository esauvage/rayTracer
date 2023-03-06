#include "raytracer.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <cmath>
#include <cstring>
#include <thread>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "utils.h"
#include "parser.h"

using namespace std;
using namespace Eigen;

int max_depth = 0;

RayTracer::RayTracer()
{
	srand(time(NULL));
}

RayTracer::~RayTracer()
{
}

void RayTracer::generateFile(const string &outFile, const pair <int, int> size, const string &inFile)
{
	chrono::time_point<chrono::system_clock> start, end;

	cout << "-----------------------------------------\n";
	start = chrono::system_clock::now();
	ifstream ifs(inFile);
	if (!ifs.good())
	{
		cout << "No valid input file." << endl;
		return;
	}
//	json data = json::parse(f);
//	scene = data.get<Scene>();
	Parser parser(ifs);
	parser.readScene(scene);
	ifs.close();
	cout << "Input file read." << endl;
	const int width {size.first};
	const int height {size.second};
//    const int num_threads = 4;
//    std::thread t[num_threads];

    //Launch a group of threads
//    for (int i = 0; i < num_threads; ++i) {
//        char * buf = img + i * width * height * 3 / num_threads;
//        t[i] = std::thread(&RayTracer::fillImage, this, buf, i * height/num_threads, height/num_threads, width, height);
//    }
//    //Join the threads with the main thread
//    for (int i = 0; i < num_threads; ++i) {
//        t[i].join();
//    }
	ofstream out(outFile);
	if (!out.good())
	{
		cout << "No valid output file." << endl;
		return;
	}
	out << "P3\n" << width << " " << height << " 255\n"; // The PPM Header is issued
//	fillImage(out, 0, 2, 2, 2);
	fillImage(out, 0, height, width, height);
	out.flush();
	out.close();
//	FILE * outF = fopen(outFile.c_str(), "w");
//	fprintf(outF, "P3\n%d %d 255\n", width, height); // The PPM Header is issued
//	char * buf = img;
//	for (auto i {width * height}; i--;)
//		fprintf(outF, "%hhu %hhu %hhu\n", buf[0], buf[1], buf[2]);
//	buf+=3;
	end = chrono::system_clock::now();

	cout << "finished calculation for "
			  << chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			  << "ms.\n";
	cout << "max light depth : " << max_depth << endl;
	ofstream o("scene.json");
	json j = scene;
	o << setw(4) << j << endl;
}

Vec3f RayTracer::pixelColor(const Rayon3f &rayon, int depth) const
{
	if (depth <= 0)
		return {0, 1, 0};//rayon.milieu->material->col * (1./255.);
	HitRecord rec;
	if (scene._world.touche(rayon, 0, INFINITY, rec))
	{
		Rayon3f scattered;
		Vec3f attenuation;
		if (rec.pMaterial->scatter(rayon, rec, attenuation, scattered))
		{
			return pixelColor(scattered, depth - 1).cwiseProduct(attenuation);
		}
		return Vec3f(0,0,1);
	}
	return sky(rayon.direction());

}

pair<float, shared_ptr<Shape>> RayTracer::nearestShape(const Rayon3f &rayon)
{
	float minDist = INFINITY;
	shared_ptr<Shape> minShape = nullptr;
//	const int num_threads = scene.shapes.size();
//	float distances[num_threads];
//	std::thread t[num_threads];
//	for (int i = 0; i < num_threads; ++i) {
//        distToShape(&distances[i], scene.shapes[i], origin, direction);
//    }
	//Launch a group of threads
//    for (int i = 0; i < num_threads; ++i) {
//        t[i] = std::thread(&RayTracer::distToShape, this, &distances[i], scene.shapes[i], origin, direction);
//    }
//	//Join the threads with the main thread
//    for (int i = 0; i < num_threads; ++i) {
//        t[i].join();
//    }
//	for (int i = 0; i < num_threads; ++i)
//	{
//		float d = distances[i];
//		if (d <= 0)
//			continue;
//		Shape * o = scene.shapes[i];
//		if ((((d < minDist)) || (minDist < 0)) && (o != excluded))
//		{
//			minShape = move(o);
//			minDist = d;
//		}
//	}
	return pair<float, shared_ptr<Shape>> {minDist, minShape};
}

void RayTracer::distToShape(float * r, shared_ptr<Shape>s, const Rayon3f &rayon)
{
//	*r = s->distance(rayon, -1);
}

Vec3f RayTracer::sky(const Vec3f& rayon) const
{
	Vec3f unit_direction = rayon.normalized();
	auto t = 0.5*(unit_direction.z() + 1.0);
	return (1.0-t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
//Plus le rayon est vertical, plus le ciel est bleu. Plus il est horizontal, plus il est blanc.
//	float coef {1.f/200.f / rayon.y()};//C'est le sinus du "vertical"
//	return {coef * 0.9f, coef, 1.f};
}

ofstream & RayTracer::fillImage(ofstream &out, int rowBegin, int nbRows, int width, int height) const
{
	const int antiAliasing {16};
//    const float halfAngle {static_cast<float>(tan(M_PI/6.))};
//	const float coef { halfAngle*2.f/max<float>(width, height)};
//	const auto point = scene.cameraPos;
//	Quaternion<float> camRot = scene.cameraRot;
//	float y = (rowBegin - height/2.f) * coef;
	for (int i = nbRows; --i;)
//		for (int i = nbRows; i--; y+=coef)
	{
//		float x = -width/2.f*coef;
		for (int j = 0; j < width; ++j)//, x+=coef)
		{
			Vec3f pixel{0, 0, 0};
			for (auto k = 0; k < antiAliasing; k++)
			{
//				const Vec3f rayon = camRot * Vec3f(x, -y, -1);
				auto u = (j + frand()) / (width);
				auto v = (i + frand()) / (height);
				auto r = scene.camera().ray(u, v);
				pixel += pixelColor(r, 16);
//				const Vec3f rayon = camRot * Vec3f(x + (frand() - 0.5f) * coef, -y + (frand() - 0.5f) * coef, -1);
//				pixel += pixelColor(Rayon3f(point, rayon), 16);
			}
			// Divide the color by the number of samples and gamma-correct for gamma=2.0.
			pixel *= 1. / (float)antiAliasing;
			pixel = pixel.array().sqrt();
			Vector3<unsigned char> pixelI = (pixel * 255).cast<unsigned char>();
			out << +pixelI.x() << " ";
			out << +pixelI.y() << " ";
			out << +pixelI.z() << endl;
		}
	}
	return out;
}

Vec3f RayTracer::rayonRefracte(Vec3f normal, Vec3f incident, float n1, float n2)
{
	//from http://raphaello.univ-fcomte.fr/ig/raytracing/lancerderayons.htm
	const float n = n1/n2;
	const float a = normal.dot(incident);
	Vec3f T = normal * (n*a - sqrt(1 - n * n * (1-(a * a)))) - incident * n;
	return T;
}
