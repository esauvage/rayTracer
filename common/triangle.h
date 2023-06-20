#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

#include <array>

class Triangle : public Shape 
{
public:
    Triangle(const std::array< Vec3f, 3>& points);
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c);
    virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const override;
private:
    std::array< Vec3f, 3 > _p;
	Vec3f _n;
	Vec3f _e1;
	Vec3f _e2;
};

#endif //TRIANGLE_H
