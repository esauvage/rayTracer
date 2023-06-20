#ifndef HORIZON_H
#define HORIZON_H

#include "shape.h"

class Horizon : public Shape
{
public:
	Horizon(float hauteur);
    virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const override;
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const override;
private:
	float _hauteur;
};

#endif // HORIZON_H
