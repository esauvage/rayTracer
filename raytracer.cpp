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
using namespace cimg_library;

int max_depth = 0;

RayTracer::RayTracer()
    :_activeThreads(0)
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
	ifstream i("scene.json");
	json data;
	i >> data;
	scene = data.get<Scene>();
	Parser parser(ifs);
	parser.readScene(scene);
	ifs.close();
	cout << "Input file read." << endl;
	const int width {size.first};
	const int height {size.second};
	CImg<unsigned char> image(width, height, 1, 3, 0);
    const int num_threads = 8;
    std::thread t[num_threads + 1];
    auto threadHeight = height/(num_threads);
    if (threadHeight * num_threads < height)
        threadHeight++;
    //Launch a group of threads
    for (int i = 0; i < num_threads; ++i) {
        _activeThreads++;
        t[i] = std::thread(&RayTracer::fillImage, this, i * threadHeight, threadHeight, &image);
    }
    CImgDisplay main_disp(image, "Generation");
    t[num_threads] = std::thread(&RayTracer::updateDisplay, this, &main_disp, &image);
    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i) {
        t[i].join();
        _activeThreads--;
    }
    t[num_threads].join();
    end = chrono::system_clock::now();
    cout << "finished calculation for "
              << chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms.\n";
    while (!main_disp.is_closed())
    {
        main_disp.wait();
    }
    image.save_bmp("test.bmp");
	ofstream o("scene.json");
	json j = scene;
	o << setw(4) << j << endl;
}

Vec3f RayTracer::pixelColor(const Rayon3f &rayon, int depth) const
{
//    Rayon3f curRay = rayon;
//    Vec3f curAttenuation(1,1, 1);
//    for (auto i = depth; --i;)
//    {
//        HitRecord rec;
//        if (scene._world.touche(curRay, 0, INFINITY, rec))
//        {
//            Rayon3f scattered;
//            Vec3f attenuation;
//            if (rec.pMaterial->scatter(curRay, rec, attenuation, scattered))
//            {
//                curRay = scattered;
//                curAttenuation = curAttenuation.cwiseProduct(attenuation);
//            }
//            else
//                return curAttenuation.cwiseProduct(Vec3f(0,0,1));
//        }
//        else
//        {
//            return curAttenuation.cwiseProduct(sky(curRay.direction()));
//        }
//    }
//    return curAttenuation;
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

Vec3f RayTracer::sky(const Vec3f& rayon) const
{
	Vec3f unit_direction = rayon.normalized();
	auto t = 0.5*(unit_direction.z() + 1.0);
	return (1.0-t)*Vec3f(1.0, 1.0, 1.0) + t*Vec3f(0.5, 0.7, 1.0);
//Plus le rayon est vertical, plus le ciel est bleu. Plus il est horizontal, plus il est blanc.
//	float coef {1.f/200.f / rayon.y()};//C'est le sinus du "vertical"
//	return {coef * 0.9f, coef, 1.f};
}

void RayTracer::fillImage(int rowBegin, int nbRows, CImg<unsigned char> *img) const
{
    const auto height = img->height();
    const auto width = img->width();
	const int antiAliasing {16};
    for (int i = nbRows + 1; --i;)
    {
        for (int j = 0; j < width; ++j)//, x+=coef)
        {
            Vec3f pixel{0, 0, 0};
            for (auto k = 0; k < antiAliasing; k++)
            {
                auto u = (j + frand()) / (width);
                auto v = (i + rowBegin + frand()) / (height);
                auto r = scene.camera().ray(u, v);
                pixel += pixelColor(r, 16);
            }
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            pixel *= 1. / (float)antiAliasing;
            pixel = pixel.array().sqrt();
            Vector3<unsigned char> pixelI = (pixel * 255).cast<unsigned char>();
            (*img)(j, height - rowBegin - i, 0, 0) = +pixelI.x();
            (*img)(j, height - rowBegin - i, 0, 1) = +pixelI.y();
            (*img)(j, height - rowBegin - i, 0, 2) = +pixelI.z();
        }
    }
}

void RayTracer::updateDisplay(CImgDisplay *display, CImg<unsigned char> *img) const
{
    while(_activeThreads)
        display->display(*img);
}
