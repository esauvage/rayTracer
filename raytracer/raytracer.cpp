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

#include "../common/utils.h"

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
    scene.updateMaterials();
    auto camera = scene.camera();
    camera.setSize(QSize(size.first, size.second));
    scene.setCamera(camera);
	cout << "Input file read." << endl;
	const int width {size.first};
	const int height {size.second};
	CImg<unsigned char> image(width, height, 1, 3, 0);
	const int num_threads = 32;
    thread t[num_threads + 1];
    CImgDisplay main_disp(image, "Generation");
    t[num_threads] = std::thread(&RayTracer::updateDisplay, this, &main_disp, &image);
//	fillImage(0, height, &image, 0);
    auto threadHeight = height/(num_threads);
	if (threadHeight * num_threads < height)
        threadHeight++;
	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) {
		_activeThreads++;
		int tHeight = min(threadHeight, height- i * threadHeight);
		t[i] = thread(&RayTracer::fillImage, this, i * threadHeight, tHeight, &image);
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
    // If the ray hits nothing, return the background color.
    if (scene.world()->touche(rayon, 0, INFINITY, rec))
	{
        //If there is no material recognised, then return the current color.
        if (!rec.pMaterial)
        {
            return attenuation;
        }
        //By default, we return what we get from emission and a black color
        const auto color_from_emission = rec.pMaterial->emitted(rec.tex(), rec.p);
        Vec3f color(0, 0, 0);
        //If the ray hits something, it may scatter
        vector<Rayon3f > vScattered;
        Rayon3f scattered;
        Vec3f localAttenuation = attenuation;//We are considering this attenuation
        if (rec.pMaterial->scatter(rayon, rec, localAttenuation, vScattered, attenuation))
        {
            auto s = 0;
            for (auto scattered : vScattered)
            {
                if (scattered.direction().hasNaN())
                {
                    cout << "scattered with NaN" << endl;
                    continue;
                }
                //We retrieve the color of the scaterred ray
                auto buf = pixelColor(scattered, depth - 1, attenuation).cwiseProduct(localAttenuation);
                if (buf.squaredNorm() == 0) continue;
                s++;
                color += buf;
            }
            color /= s ? s : 1;
            //			if ((color.array() < 0).any())
            //			{
            //				cout << "Erreur scattered Color : " << color << endl;
            //				color = Vec3f(fmax(0, color[0]), fmax(0, color[1]), fmax(0, color[2]));
            //            }
        }
        return color_from_emission + color;
    }
//	file << "No touch." << endl;
	return sky(rayon.direction());
}

Vec3f RayTracer::sky(const Vec3f& rayon) const
{
    //On ne touche rien, donc on tape dans le ciel.
    //L'éclairage doit donc prendre en compte l'inclinaison
    Vec3f unit_direction = rayon.stableNormalized();
	float t = 0.5*(unit_direction.z() + 1.0);
	Vec3f v = (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    Vec3f sky(0.2, 0.2, 0.2);//ambiant Light
    int realSuns = 0;
    for (const auto &sun : scene.suns())
	{
        float intensity = 1;
        if (sun.direction().array().isInf().any() || sun.direction().hasNaN())
        {
            intensity = sun.intensity();
        }
        else
        {
            intensity = fmax(unit_direction.dot(sun.direction()), 0.) * sun.intensity();
        }
        if (intensity <= 0)
            continue;
        sky += v.cwiseProduct(sun.color()) * intensity;
        realSuns++;
	}
    if (realSuns)
    {
        sky /= (float)realSuns;
    }
	return sky;
//Plus le rayon est vertical, plus le ciel est bleu. Plus il est horizontal, plus il est blanc.
//	float coef {1.f/200.f / rayon.y()};//C'est le sinus du "vertical"
//	return {coef * 0.9f, coef, 1.f};
}

void RayTracer::fillImage(int rowBegin, int nbRows, CImg<unsigned char> *img) const
{
    const float height = img->height();
    const float width = img->width();
    const int antiAliasing {5};
    for (int i = nbRows -1; i >= 0;--i)
    {
        for (int j = 0; j < width; ++j)//, x+=coef)
        {
            Vec3f pixel{0, 0, 0};
            int realAntiAliasing = 0;
			for (auto k = 0; k < antiAliasing; k++)
            {
				auto u = (j + (k / (float)(antiAliasing - 1)) - 0.5)/ width;//(j + frand() - 0.5) / (width);
				for (auto l = 0; l < antiAliasing; l++)
				{
                    auto v = (i + rowBegin + (l / (float)(antiAliasing - 1)) - 0.5) / (height);
                    auto r = scene.camera().ray(u, v);
					while (r.direction().hasNaN())
                    {
                        cout << "Scene camera generated a NaN" << std::endl;
                        scene.camera().ray(u, v);
					}
					Vec3f attenuation(1., 1., 1.);
                    auto localPixel = pixelColor(r, 16, attenuation);
                    if (localPixel.squaredNorm() == INFINITY)
                        continue;
                    realAntiAliasing++;
					pixel += localPixel;
//					myfile << "row " << i << "col" << j << endl;
				}
            }
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            pixel *= 1. / (float)realAntiAliasing;
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
}

void RayTracer::updateDisplay(CImgDisplay *display, CImg<unsigned char> *img) const
{
    while(_activeThreads)
	{
        display->display(*img);
		display->wait(5);
	}
}
