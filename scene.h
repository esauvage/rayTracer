#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <list>

#include "shape.h"
#include "light.h"
//#include "quaternion.h"

class Scene : public Shape
{
public:
	Scene();
	Scene(std::shared_ptr<Shape> object);
	~Scene();
	void clear();
	void add(std::shared_ptr<Shape> object);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, hit_record& rec) const override;
	virtual float distance(const Rayon3f & ray, float minDist = -1) const;
	virtual float distanceMin(const Vec3f & origin) const;
	virtual Vec3f normal(const Vec3f & p) const;
	std::vector <std::shared_ptr<Shape> > shapes;
	std::vector <Light *> lights;
	std::map <std::string, Material>materials;
	Vec3f cameraPos{0, 0, 1};
	Eigen::Quaternion<float> cameraRot{1, 0, 0, 0};
};

#endif
