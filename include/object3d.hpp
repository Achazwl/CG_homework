#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}
    explicit Object3D(Material *material) { this->material = material; }
    virtual ~Object3D() = default;

    virtual bool intersect(const Ray &ray, Hit &hit, float tmin) = 0;

protected:

    Material *material;
};

#endif

