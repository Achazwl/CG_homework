#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <algorithm>
#include <glut.h>

class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {
    }
    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const { return diffuseColor; }

    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor) {
        auto tmp = Vector3f::dot(hit.getNormal(), dirToLight);
        auto diffuse = std::max<float>(0, tmp);
        auto R = 2 * tmp  * hit.getNormal() - dirToLight;
        auto specular = pow(std::max<float>(0, Vector3f::dot(-ray.getDirection(), R)), shininess);
        return lightColor * (diffuse * diffuseColor + specular * specularColor);
    }

    void Use() { 
        // 材质应用于正反两面， RGBA(reg, green, blue, alpha)
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f)); // 散射
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f)); // 镜面反射
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f)); // 镜面反射指数 // TODO why two value are passed??????
    }

protected:
    Vector3f diffuseColor; // 漫反射
    Vector3f specularColor; // 镜面反射
    float shininess; // 光泽度
};


#endif // MATERIAL_H
