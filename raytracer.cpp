#include "raytracer.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <cmath>
#include <cstring>
#include <thread>

#include "parser.h"
#include "boule.h"
#include "rayon.h"

const double M_PI = 3.141592653589793238463;    //value of pi

int depth = 0;
int max_depth = 0;
float frand() {return rand()/static_cast<float>(RAND_MAX);}

RayTracer::RayTracer()
{
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
		std::cout << "No valid input file." << std::endl;
		return;
	}
	Parser parser(ifs);
	parser.readScene(scene);
	ifs.close();
	std::cout << "Input file read." << std::endl;
	const int width {size.first};
	const int height {size.second};
	char *img = (char *)malloc(width * height * 3* sizeof(char));
    const int num_threads = 4;
    std::thread t[num_threads];

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
	out << "P3\n" << width << " " << height << " 255\n"; // The PPM Header is issued
	fillImage(out, 0, height, width, height);
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
	free(img);
}

Vec3f RayTracer::pixelColor(Rayon3f rayon)
{
	if (depth > 12)
		return {0, 0, 0};//rayon.milieu->material->col * (1./255.);
	depth++;
	max_depth = std::max(max_depth, depth);
	auto p = nearestShape(rayon);
	const float minDist = p.first;
	Shape * minShape = p.second;
	if (!minShape)
	{
		depth--;
		return sky(rayon.direction());
// 		return Vec3f{0, 0, 0};
	}
	const Vec3f impact = rayon.origin() + rayon.direction() * minDist;
	const Vec3f normale = minShape->normal(impact);
	const Vec3f reflexion = rayon.direction() - normale * (rayon.direction().dot(normale)) * 2;
	Vec3f couleur(0, 0, 0);
	for (auto l:scene.lights)
	{
		float coef {0};
		const int softShadow{8};
		for (int k = 0; k < softShadow; k++)
		{
			const Vec3f lPos((l->pos + (Vec3f(frand() - 0.5f, frand() - 0.5f, frand() - 0.5f) * 0.01f)).normalized());
			p = nearestShape(Rayon3f(impact, lPos, minShape));
			if (p.second)
			{
				continue;
			}
			auto s = lPos.dot(normale);

			coef += max<float>(s, 0); // Partie diffusion
			Vec3f RLight = lPos - normale * (s) * 2;
			const auto dot = max<float>(0., rayon.direction().dot(RLight));
			coef += pow(dot, 20) * 0.2f; //Partie spéculaire
		}
		couleur = couleur + ((minShape->material->col * (1.f/255.f)).cwiseProduct(l->col) * (coef /static_cast<float>(softShadow)));
	}
    couleur = couleur * (1.f/scene.lights.size());
	if (minShape->material->reflectance)
		couleur = couleur + pixelColor(Rayon3f(impact, reflexion, minShape)) * minShape->material->reflectance;
//	if (minShape->material->transparence)
//	{
//		float indiceRefrac = 1;
//		if (minShape != rayon.milieu)
//			indiceRefrac = minShape->material->refraction;
//		const Vec3f transmission = rayonRefracte(normale, rayon.direction,
//												   rayon.milieu ? rayon.milieu->material->refraction : 1,
//												   indiceRefrac);
//		couleur = couleur * (1. - minShape->material->transparence)
//				+ pixelColor(Rayon(impact, transmission, minShape)) * minShape->material->transparence;
//	}
	depth--;
	return couleur.cwiseMin(Vec3f({1,1,1}));
//	return couleur.borne({1,1,1});
// 	return (couleur * (1./scene.lights.size()) + recursion * minShape->material->reflectance).borne(Vec3f(1, 1, 1));
}

pair<float, Shape *> RayTracer::nearestShape(const Rayon3f &rayon)
{
	float minDist = INFINITY;
	Shape * minShape = nullptr;
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
	for (Shape * o:scene.shapes)
	{
		float d;
		if (o == rayon.milieu())// && o->material->transparence)
			d = o->distanceMax(rayon.origin(), rayon.direction(), minDist);
		else
		{
			if ((o == rayon.milieu()) || (o->distanceMin(rayon.origin()) > minDist))
				continue;
			d = o->distance(rayon.origin(), rayon.direction(), minDist);
		}
		if (d <= 0)
			continue;
		if ((d < minDist) || (minDist < 0))
		{
			minShape = o;
			minDist = d;
		}
	}
	return pair<float, Shape *> {minDist, minShape};
}

void RayTracer::distToShape(float * r, Shape * s, Vec3f origin, Vec3f direction)
{
	*r = s->distance(origin, direction, -1);
}

Vec3f RayTracer::sky(const Vec3f& rayon)
{
//	Vec3f unit_direction = rayon.normalized();
//	auto t = 0.5*(unit_direction.z() + 1.0);
//	return (1.0-t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
//Plus le rayon est vertical, plus le ciel est bleu. Plus il est horizontal, plus il est blanc.
	float coef {1.f/20.f / rayon.z()};//C'est le sinus du "vertical"
	return {coef * 0.9f, coef, 1.f};
}

void RayTracer::fillImage(ofstream &out, int rowBegin, int nbRows, int width, int height)
{
    const float halfAngle {static_cast<float>(tan(M_PI/6.))};
	const float coef { halfAngle*2.f/max<float>(width, height)};
	const Vec3f point{scene.cameraPos};
	Quaternion<float> camRot = scene.cameraRot;
    for (float i {static_cast<float>(nbRows)}, x {(rowBegin - height/2.f) * coef}; i--; x+=coef)
	{
        for (float j = 0, y = -width/2.f*coef; j < width; ++j, y+=coef)
		{
			Vec3f pixel{0, 0, 0};
			const int antiAliasing {16};
			for (auto k = 0; k < antiAliasing; k++)
			{
				depth = 0;
				const Vec3f rayon = camRot.rotate(Vec3f(1, -y + (frand() - 0.5f) * coef,
									-x + (frand() - 0.5f) * coef).normalized());
				pixel = pixel + pixelColor(Rayon3f(point, rayon, nullptr)) * (1. / antiAliasing);
			}
			Eigen::Vector3<unsigned char> pixelI = (pixel * 255.99).cast<unsigned char>();
			out << +pixelI.x() << " ";
			out << +pixelI.y() << " ";
			out << +pixelI.z() << endl;
		}
	}
}

Vec3f RayTracer::rayonRefracte(Vec3f normal, Vec3f incident, float n1, float n2)
{
	//from http://raphaello.univ-fcomte.fr/ig/raytracing/lancerderayons.htm
	const float n = n1/n2;
	const float a = normal.dot(incident);
	Vec3f T = normal * (n*a - sqrt(1 - n * n * (1-(a * a)))) - incident * n;
	return T;
}
