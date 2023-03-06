#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"

using Rayon3f = Rayon<float, 3>;

class Material;

class HitRecord;

class Shape
{
public:
	virtual ~Shape(){}
	virtual bool touche(const Rayon3f& r, double t_min, double t_max, HitRecord& rec) const = 0;
	std::shared_ptr<Material> material() const;

	void setMaterial(const std::shared_ptr<Material> &newMaterial);

	std::string nom() const;
	void setNom(const std::string &newNom);

private:
	std::shared_ptr<Material>_material {nullptr};
	std::string _nom;
};

inline std::shared_ptr<Material> Shape::material() const
{
	return _material;
}

inline void Shape::setMaterial(const std::shared_ptr<Material> &newMaterial)
{
	_material = newMaterial;
}

inline std::string Shape::nom() const
{
	return _nom;
}

inline void Shape::setNom(const std::string &newNom)
{
	_nom = newNom;
}
#endif // SHAPE
