#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Plane : public Object3D {
public:
    Plane() = delete;
    // given by ax+by+cz=d, tranfer to ax+by+cz+d=0
    Plane(const Vector3f &norm, float d, Material *m) : Object3D(m), norm(norm.normalized()), d(-d) { }
    ~Plane() override = default;

    bool intersect(const Ray& ray, Hit &hit, float tmin) override {
        auto b = Vector3f::dot(norm, ray.getDirection());
        if (fabs(b) < 1e-7) return false;
        auto t = -(d + Vector3f::dot(norm, ray.getOrigin())) / b;
        if (t < tmin || t > hit.getT()) return false;
        hit.set(t, material, norm);
        return true;
    }

    void drawGL() override {
        Object3D::drawGL(); // base class func
        
        auto yAxis = Vector3f::cross(norm, Vector3f::RIGHT); // any vector orthogonal to norm is ok
        auto xAxis = Vector3f::cross(yAxis, norm);
        // use d * norm as plane center, build x-y coordinate on that plane
        const float planeSize = 10.0; // large finite plane instead of infinite plane
        glBegin(GL_TRIANGLES); { // use 2 triangle instead of QUADS, since GPU render triangle
            glNormal3fv(norm);
            glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
            glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
            glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
            glNormal3fv(norm);
            glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
            glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
            glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        } glEnd();
    }

protected:
    // function: ax+by+cz+d=0 -> n dot p + d = 0
    Vector3f norm;
    float d;
};

#endif //PLANE_H