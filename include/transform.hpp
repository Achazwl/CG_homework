#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"

// 用逆变换的射线和obj求交, 由于变换后的direction不再是单位向量， 所以包含了距离的变换信息
// 并求出其原法向量，再用法向量变换成变换后obj的法向量
class Transform : public Object3D {
public:
    Transform() = delete;
    Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
        transform = m.inverse();
    }
    ~Transform() override = default;

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        Vector3f trSource = transformPoint(transform, ray.getOrigin());
        Vector3f trDirection = transformDirection(transform, ray.getDirection());
        Ray tr(trSource, trDirection);
        bool inter = o->intersect(tr, hit, tmin);
        if (inter) {
            hit.set(hit.getT(), hit.getMaterial(), transformDirection(transform.transposed(), hit.getNormal()).normalized());
        }
        return inter;
    }

    static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
    // transforms a 3D point using a matrix, returning a 3D point
        return (mat * Vector4f(point, 1)).xyz();
    }

    static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
    // transform a 3D direction using a matrix, returning a direction
        return (mat * Vector4f(dir, 0)).xyz();
    }

protected:
    Object3D *o; //un-transformed object
    Matrix4f transform;
};

#endif //TRANSFORM_H
