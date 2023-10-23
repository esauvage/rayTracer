#ifndef SHAPE
#define SHAPE

#include "material.h"
#include "rayon.h"
#include "aabb.h"
#include <QByteArray>

class HitRecord;

class Shape
{
public:
	virtual ~Shape(){}
	virtual bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const = 0;
    virtual bool boundingBox(double time0, double time1, AABB& output_box) const {(void)output_box;(void)time0;(void)time1;return false;}
	std::shared_ptr<Material> material() const;

	void setMaterial(const std::shared_ptr<Material> &newMaterial);
    virtual nlohmann::json &jsonHelper(nlohmann::json& j) const
	{
		if (_material)
            j= nlohmann::json{{"material", _material->nom()}};
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

inline void to_json(nlohmann::json& j, const Shape& s)
{
	j = s.jsonHelper(j);
}

//namespace nlohmann {
//    template <typename T>
//    struct adl_serializer<std::shared_ptr<T> > {
//        static void to_json(nlohmann::json& j, const std::shared_ptr<T>& p) {
//            j = nlohmann::json(*p);
//        }

//        static void from_json(const nlohmann::json& j, std::shared_ptr<T>& p) {
//                *p = j.get<T>(); // same as above, but with
//                                  // adl_serializer<T>::from_json
//            }
//        };
//}

#endif // SHAPE
