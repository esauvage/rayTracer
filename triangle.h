#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

#include <array>

class Triangle : public Shape 
{
public:
    Triangle(const std::array< Vec3f, 3>& points);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, std::ofstream& file) const;
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const;
private:
    std::array< Vec3f, 3 > _p;
	Vec3f _n;
	Vec3f _e1;
	Vec3f _e2;
};

#endif //TRIANGLE_H
