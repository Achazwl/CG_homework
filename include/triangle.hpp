#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

class Triangle: public Object3D {

public:
	Triangle() = delete;
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m), vertices{a,b,c} {
        Vector3f x = vertices[1] - vertices[0];
        Vector3f y = vertices[2] - vertices[0];
        y = Vector3f::cross(a, b);
        normal = y.normalized()
    }

	bool intersect(const Ray& ray,  Hit& hit, float tmin) override {
	    auto E1 = vertices[0] - vertices[1];
        auto E2 = vertices[0] - vertices[2];
	    auto S = vertices[0] - ray.getOrigin();
	    auto div = Matrix3f::determinant3x3(
            ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2],
            E1[0], E1[1], E1[2],
            E2[0], E2[1], E2[2]
        );
	    if (fabs(div) < 1e-7) return false;
	    auto t = Matrix3f::determinant3x3(
            S[0], S[1], S[2],
            E1[0], E1[1], E1[2],
            E2[0], E2[1], E2[2]
        ) / div;
	    if (t < tmin || t > hit.getT()) return false;
        auto a = Matrix3f::determinant3x3(
                ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2],
                S[0], S[1], S[2],
                E2[0], E2[1], E2[2]
        ) / div;
        if (a < 0 || a > 1) return false;
        auto b = Matrix3f::determinant3x3(
                ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2],
                E1[0], E1[1], E1[2],
                S[0], S[1], S[2]
        ) / div;
        if (b < 0 || b > 1) return false;
        if (a + b > 1) return false;

        hit.set(t, material, normal);
        return true;
	}
protected:
    Vector3f normal;
    Vector3f vertices[3];
};

#endif //TRIANGLE_H
