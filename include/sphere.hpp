#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() : center(), radius(1) { }
    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material), center(center), radius(radius) { }
    ~Sphere() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        auto l = center - ray.getOrigin();
        return false;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
