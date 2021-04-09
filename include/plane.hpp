#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Plane : public Object3D {
public:
    Plane() = delete;
    // given by ax+by+cz=d, tranfer to ax+by+cz+d=0
    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m), normal(normal.normalized()), d(-d) { }
    ~Plane() override = default;

    bool intersect(const Ray& ray, Hit &hit, float tmin) override {
        auto b = Vector3f::dot(normal, ray.getDirection());
        if (fabs(b) < 1e-7) return false;
        auto t = -(d + Vector3f::dot(normal, ray.getOrigin())) / b;
        if (t < tmin || t > hit.getT()) return false;
        hit.set(t, material, normal);
        return true;
    }

    void drawGL() override { // TODO what
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

protected:
    // function: ax+by+cz+d=0 -> n dot p + d = 0
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

