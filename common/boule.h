#ifndef BOULE_H
#define BOULE_H

#include "shape.h"
#include "utils.h"
#include <QByteArray>
#include <QVector3D>

class Boule : public Shape
{
public:
	Boule(const Vec3f& position, float rayon);
    bool touche(const Rayon3f& r, float t_min, float t_max, HitRecord& rec) const override;
    bool boundingBox(double time0, double time1, AABB& output_box) const override;
    nlohmann::json &jsonHelper(nlohmann::json& j) const override;
    int serialize(QByteArray &dest);
private:
    Vec2f UV(const Vec3f &p) const;
    void toIcosahedron(QList<Vec3f> &icosahedron);

    Vec3f _pos;
    float _r;
};

#endif // BOULE_H
