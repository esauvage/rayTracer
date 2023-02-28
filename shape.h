#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"

using Rayon3f = Rayon<float, 3>;
using namespace std;

class Material;

class HitRecord {
public:
	Vec3f p;
	Vec3f normal;
	float t;
	bool front_face;
	shared_ptr<Material> pMaterial;

	inline void setFaceNormal(const Rayon3f& r, const Vec3f& outward_normal) {
		front_face = r.direction().dot(outward_normal) < 0;
		normal = front_face ? outward_normal :-outward_normal;
	}
};

class Shape
{
public:
	virtual ~Shape(){}
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const = 0;
	virtual float distance(const Rayon3f & ray, float minDist = -1) const {return INFINITY;}
	virtual float distanceMax(const Rayon3f & r, float minDist = -1) const	{return distance(r, minDist);}
	virtual float distanceMin(const Vec3f & origin) const {return INFINITY;}
	virtual Vec3f normal(const Vec3f & p) const {return -p;}
	shared_ptr<Material> material() const;

	void setMaterial(const shared_ptr<Material> &newMaterial);

private:
	shared_ptr<Material>_material {nullptr};
};

inline shared_ptr<Material> Shape::material() const
{
	return _material;
}

inline void Shape::setMaterial(const shared_ptr<Material> &newMaterial)
{
	_material = newMaterial;
}
#endif // SHAPE
