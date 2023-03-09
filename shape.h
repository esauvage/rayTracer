#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
	virtual json &jsonHelper(json& j) const
	{
		if (_material)
			j= json{{"material", _material->nom()}};
		return j;
	}

private:
	std::shared_ptr<Material>_material {nullptr};
};

inline std::shared_ptr<Material> Shape::material() const
{
	return _material;
}

inline void Shape::setMaterial(const std::shared_ptr<Material> &newMaterial)
{
	_material = newMaterial;
}

inline void to_json(json& j, const Shape& s)
{
	j = s.jsonHelper(j);
}

namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T> > {
        static void to_json(json& j, const std::shared_ptr<T>& p) {
            j = json(*p);
        }

        static void from_json(const json& j, std::shared_ptr<T>& p) {
                *p = j.get<T>(); // same as above, but with
                                  // adl_serializer<T>::from_json
            }
        };
}

#endif // SHAPE
