#include "raytracer.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <cmath>
#include <cstring>
#include <thread>
#include <mutex>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "utils.h"

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
//	ifstream ifs(inFile);
//	if (!ifs.good())
//	{
//		cout << "No valid input file." << endl;
//		return;
//	}
//	Parser parser(ifs);
//	parser.readScene(scene);
//	ifs.close();
	ifstream i(inFile);
	json data;
	i >> data;
	scene = data.get<Scene>();
	i.close();
	cout << "Input file read." << endl;
	const int width {size.first};
	const int height {size.second};
	CImg<unsigned char> image(width, height, 1, 3, 0);
    const int num_threads = 32;
    thread t[num_threads + 1];
    CImgDisplay main_disp(image, "Generation");
    t[num_threads] = std::thread(&RayTracer::updateDisplay, this, &main_disp, &image);
//    fillImage(0, height, &image);
    auto threadHeight = height/(num_threads);
    if (threadHeight * num_threads < height)
        threadHeight++;
    //Launch a group of threads
    for (int i = 0; i < num_threads; ++i) {
        _activeThreads++;
        int tHeight = min(threadHeight, height- i * threadHeight);
        t[i] = thread(&RayTracer::fillImage, this, i * threadHeight, tHeight, &image, i);
    }
	//Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i) {
        t[i].join();
        _activeThreads--;
    }
    t[num_threads].join();
	end = chrono::system_clock::now();
	cout << "finished calculation for "
              << chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
         << "ms.\n" << endl;
    while (!main_disp.is_closed())
    {
        main_disp.wait();
    }
    image.save(outFile.c_str());
    ofstream o((inFile+".bak").c_str());
	json j = scene;
	o << setw(4) << j << endl;
    o.close();
}

Vec3f RayTracer::pixelColor(const Rayon3f &rayon, int depth, Vec3f &attenuation) const
{
	if (depth <= 0)
    {
		return sky(rayon.direction());//{0, 1, 0};//rayon.milieu->material->col * (1./255.);
    }
	HitRecord rec;
    if (scene.world()->touche(rayon, 0, INFINITY, rec))
	{
		vector<Rayon3f > vScattered;
		Rayon3f scattered;
		Vec3f localAttenuation = attenuation;
		if (rec.pMaterial && rec.pMaterial->scatter(rayon, rec, localAttenuation, vScattered, attenuation))
		{
			Vec3f color(0, 0, 0);
			for (auto &scattered : vScattered)
			{
                color+= pixelColor(scattered, depth - 1, attenuation).cwiseProduct(localAttenuation);
			}
			color /= vScattered.size();
			if ((color.array() < 0).any())
			{
				cout << "Erreur scattered Color : " << localAttenuation << endl;
			}
			return color;
		}
//		return rec.normal()*0.5 + Vec3f(0.5, 0.5, 0.5);
        return localAttenuation;
	}
//	file << "No touch." << endl;
	return sky(rayon.direction());
}

Vec3f RayTracer::sky(const Vec3f& rayon) const
{
    //On ne touche rien, donc on tape dans le ciel.
    //L'éclairage doit donc prendre en compte l'inclinaison
    Vec3f unit_direction = rayon.normalized();
    Vec3f soleil_direction(1.0f, 1.0f, 0.8f);
	Vec3f couleur_soleil(1.f, 1.0f, 0.95f);
	soleil_direction = soleil_direction.normalized();
	float t = 0.5*(unit_direction.z() + 1.0);
	float intensite_soleil = fmax(unit_direction.dot(soleil_direction), 0.);
	float intensite_ambiante = 0.3;
	Vec3f v = (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
	Vec3f sky = (v * intensite_ambiante + couleur_soleil * intensite_soleil)/(1 + intensite_ambiante);
	return sky;
//Plus le rayon est vertical, plus le ciel est bleu. Plus il est horizontal, plus il est blanc.
//	float coef {1.f/200.f / rayon.y()};//C'est le sinus du "vertical"
//	return {coef * 0.9f, coef, 1.f};
}

void RayTracer::fillImage(int rowBegin, int nbRows, CImg<unsigned char> *img, int id) const
{
	ofstream myfile;
	auto fileName = "output" + to_string(id) + ".txt";
	myfile.open (fileName);

    const float height = img->height();
    const float width = img->width();
    const int antiAliasing {5};
    for (int i = nbRows + 1; --i;)
    {
        for (int j = 0; j < width; ++j)//, x+=coef)
        {
            Vec3f pixel{0, 0, 0};
			for (auto k = 0; k < antiAliasing; k++)
            {
				auto u = (j + (k / (float)(antiAliasing - 1)) - 0.5)/ width;//(j + frand() - 0.5) / (width);
				for (auto l = 0; l < antiAliasing; l++)
				{
					auto v = (i + rowBegin + (l / (float)(antiAliasing - 1)) - 0.5) / (height);
					auto r = scene.camera().ray(u, v);
					Vec3f attenuation(1., 1., 1.);
                    auto localPixel = pixelColor(r, 16, attenuation);
					pixel += localPixel;
//					myfile << "row " << i << "col" << j << endl;
				}
            }
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
			pixel *= 1. / (float)(antiAliasing * antiAliasing);
			pixel = pixel.array().min(1.);
			pixel = pixel.array().max(0.);
			pixel = pixel.array().sqrt();
			Vector3<unsigned char> pixelI = (pixel * 255).cast<unsigned char>();
//			std::lock_guard<std::mutex> guard(img_mutex);
			(*img)(j, i + rowBegin, 0, 0) = +pixelI.x();
            (*img)(j, i + rowBegin, 0, 1) = +pixelI.y();
            (*img)(j, i + rowBegin, 0, 2) = +pixelI.z();
        }
		cout << "row " << i << endl;
    }
	myfile.close();
}

void RayTracer::updateDisplay(CImgDisplay *display, CImg<unsigned char> *img) const
{
    while(_activeThreads)
	{
        display->display(*img);
		display->wait(5);
	}
}
