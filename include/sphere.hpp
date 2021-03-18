#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Sphere : public Object3D {
public:
    Sphere() : center(), radius(1) { }
    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material), center(center), radius(radius) { }
    ~Sphere() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        Vector3f l = center - ray.getOrigin(); // 3 pm
        auto tp = Vector3f::dot(l, ray.getDirection());
        auto d = radius * radius - l.length() + tp * tp;
        if (d < 0) return false;
        auto t = tp - sqrt(d);
        if (t < tmin || t > hit.getT()) return false;
        hit.set(t, material, (t * ray.getDirection() - center).normalized());
        return true;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
