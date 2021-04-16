#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"

// 用视角的逆变换， 代替物体的变换
class Transform : public Object3D {
public:
    Transform() = delete;
    Transform(const Matrix4f &m, Object3D *obj) : o(obj), transform(m.inverse()) { }
    ~Transform() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        Vector3f trSource = transformPoint(transform, ray.getOrigin());
        Vector3f trDirection = transformDirection(transform, ray.getDirection());
        Ray tr(trSource, trDirection);

        tmin *= scale;
        hit.setT(hit.getT() * scale);
        bool inter = o->intersect(tr, hit, tmin);
        hit.setT(hit.getT() / scale);
        if (inter) {
            hit.set(hit.getT(), hit.getMaterial(), transformDirection(transform.transposed(), hit.getNormal()));
        }
        return inter;
    }

    Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
        return (mat * Vector4f(point, 1)).xyz();
    }

    Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
        auto vec = (mat * Vector4f(dir, 0)).xyz();
        this->scale = vec.length();
        return vec / this->scale;
    }

    void drawGL() override {
        Object3D::drawGL(); // base class func
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); {
            glMultMatrixf(transform.inverse()); // 视角逆变换
            o->drawGL(); // 在逆变换的视角下看物体
        } glPopMatrix(); // 还原视角
    }

protected:
    Object3D *o; //un-transformed object
    Matrix4f transform;
    float scale;
};

#endif //TRANSFORM_H
