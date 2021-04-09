#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"

// 用逆变换的射线和obj求交, 注意变换后射线长度的问题
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

    void drawGL() override { // TODO what
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glMultMatrixf(transform.inverse());
        o->drawGL();
        glPopMatrix();
    }

protected:
    Object3D *o; //un-transformed object
    Matrix4f transform;
    float scale;
};

#endif //TRANSFORM_H
