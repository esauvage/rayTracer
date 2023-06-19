#ifndef BOULE_H
#define BOULE_H

#include "shape.h"
#include "utils.h"

class Boule : public Shape
{
public:
	Boule(const Vec3f& position, float rayon);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, std::ofstream& file) const override;
	virtual bool boundingBox(double time0, double time1, Eigen::AlignedBox3f& output_box) const override;
	virtual json &jsonHelper(json& j) const override;
private:
	Vec2f getUV(const Vec3f &p) const;

	Vec3f _pos;
	float _r;
};

#endif // BOULE_H
