#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Plane : public Object3D {
public:
    Plane() = delete;
    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m), normal(normal), d(d) { }
    ~Plane() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        auto b = Vector3f::dot(normal, ray.getDirection());
        if (fabs(b) < 1e-7) return false;
        auto t = -(d + b) / b;
        if (t < tmin || t > hit.getT()) return false;
        hit.set(t, material, normal);
        return true;
    }

protected:
    // function: ax+by+cz+d=0 -> n dot p + d = 0
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

