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
        Vector3f l = center - ray.getOrigin();
        auto tp = Vector3f::dot(l, ray.getDirection());
        auto d = radius * radius - Vector3f::dot(l, l) + tp * tp;
        if (d < 0) return false;
        auto t = tp - sqrt(d);
        if (t < tmin || t > hit.getT()) return false;
        hit.set(t, material, (ray.pointAtParameter(t) - center).normalized());
        return true;
    }

    void drawGL() override {
        Object3D::drawGL(); // base class func
        
        glMatrixMode(GL_MODELVIEW);
        // 画圆没有指定圆画的函数，所以只能先将左边中心移过去，画完再把座标中心还原
        glPushMatrix(); { // save cur coordinate location
            glTranslatef(center.x(), center.y(), center.z()); // move to position
            glutSolidSphere(radius, 80, 80); // use latitude & longtitude * QUADS to render sphere
        } glPopMatrix(); // restore coordinate location
    }

protected:
    Vector3f center;
    float radius;
};


#endif
