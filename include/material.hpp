#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <algorithm>

class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }


    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        auto tmp = Vector3f::dot(hit.getNormal(), dirToLight);
        auto diffuse = std::max<float>(0, tmp);
        auto R = 2 * tmp  * hit.getNormal() - dirToLight;
        auto specular = pow(std::max<float>(0, Vector3f::dot(-ray.getDirection(), dirToLight)), shininess);
        return diffuse * diffuseColor + specular * specularColor;
    }

protected:
    Vector3f diffuseColor; // 漫反射
    Vector3f specularColor; // 镜面反射
    float shininess; // 光泽度
};


#endif // MATERIAL_H
