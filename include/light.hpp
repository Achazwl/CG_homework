#ifndef LIGHT_H
#define LIGHT_H

#include <Vector3f.h>
#include "object3d.hpp"

class Light {
public:
    Light() = default;

    virtual ~Light() = default;

    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const = 0;
    virtual void turnOn(int idx) const = 0; // idx 0~7
};


class DirectionalLight : public Light { // 平行光
public:
    DirectionalLight() = delete;

    DirectionalLight(const Vector3f &d, const Vector3f &c) {
        direction = d.normalized();
        color = c;
    }

    ~DirectionalLight() override = default;

    void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const override {
        // 从给定点p的角度看，光的dir和col（通过引用返回）
        dir = -direction; // a;ready normalized
        col = color;
    }

    void turnOn(int idx) const override {
        glEnable(GL_LIGHT0 + idx);
        // 最后一个参数0~1为距离衰减, 0表示方向光
        glLightfv(GL_LIGHT0 + idx, GL_DIFFUSE, Vector4f(color, 1.0));
        glLightfv(GL_LIGHT0 + idx, GL_SPECULAR, Vector4f(color, 1.0));
        glLightfv(GL_LIGHT0 + idx, GL_POSITION, Vector4f(-direction, 0.0));
    }

private:

    Vector3f direction;
    Vector3f color;

};

class PointLight : public Light { // 点光源
public:
    PointLight() = delete;

    PointLight(const Vector3f &p, const Vector3f &c) {
        position = p;
        color = c;
    }

    ~PointLight() override = default;

    void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const override {
        // 从给定点p的角度看，光的dir和col（通过引用返回）
        dir = (position - p);
        dir = dir / dir.length();
        col = color;
    }

    void turnOn(int idx) const override { 
        glEnable(GL_LIGHT0 + idx);
        // 最后一个参数0~1为距离衰减, 1表示不衰减
        glLightfv(GL_LIGHT0 + idx, GL_DIFFUSE, Vector4f(color, 1.0));
        glLightfv(GL_LIGHT0 + idx, GL_SPECULAR, Vector4f(color, 1.0));
        glLightfv(GL_LIGHT0 + idx, GL_POSITION, Vector4f(position, 1.0));
    }

private:

    Vector3f position;
    Vector3f color;

};

#endif // LIGHT_H
