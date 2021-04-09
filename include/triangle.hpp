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
        this->E1 = vertices[0] - vertices[1];
        this->E2 = vertices[0] - vertices[2];
        normal = Vector3f::cross(E1, E2).normalized();
    }

	bool intersect(const Ray& ray,  Hit& hit, float tmin) override {
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

        // Vector3f::dot(ray.dir, normal) == det(ray.dir, E1, E2) where normal=cross(E1, E2)
        int sgn = div < 0 ? 1 : -1;
        hit.set(t, material, normal * sgn);
        return true;
	}

    void drawGL() override { // TODO what
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

protected:
    Vector3f normal;
    Vector3f vertices[3];
    Vector3f E1, E2;
};

#endif //TRIANGLE_H
