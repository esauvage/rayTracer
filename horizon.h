#ifndef HORIZON_H
#define HORIZON_H

#include "shape.h"

class Horizon : public Shape
{
public:
	Horizon(float hauteur);
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec, std::ofstream& file) const;
	virtual json &jsonHelper(json& j) const;
private:
	float _hauteur;
};

#endif // HORIZON_H
