#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "transform.hpp"
#include <vecmath.h>
#include <cmath>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->_z = direction.normalized();
        this->y = up.normalized();
        this->x = Vector3f::cross(this->_z, y).normalized();
        this->y = Vector3f::cross(this->x, this->_z);
        this->width = imgW;
        this->height = imgH;
    }
    virtual ~Camera() = default;

    virtual Ray generateRay(const Vector2f &point) = 0; // Generate rays for each screen-space coordinate

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    Vector3f center;
    Vector3f x, y, _z; // X, Y, -Z

    int width;
    int height;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH, float angle)
        : Camera(center, direction, up, imgW, imgH) {
        this->R = Matrix3f(
            x[0], y[0], -_z[0],
            x[1], y[1], -_z[1],
            x[2], y[2], -_z[2]
        );
        auto real = tanf(angle/2.0f);
        this->fx = real / (width/2.0f);
        this->fy = real / (height/2.0f);
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f cameraview((point.x()-width/2.0)*fx, (point.y()-height/2.0)*fy, -1);
        return Ray(center, (R * cameraview).normalized());
    }

protected:
    Matrix3f R; // rotation
    float fx, fy;
};

#endif //CAMERA_H
