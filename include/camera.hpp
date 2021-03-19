#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "transform.hpp"
#include <vecmath.h>
#include <cmath>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center; // origin
        this->_z = direction.normalized();
        this->y = up.normalized();
        this->x = Vector3f::cross(this->_z, up);
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
        this->R = Matrix4f(
            x[0], y[0], -_z[0], 0,
            x[1], y[1], -_z[1], 0,
            x[2], y[2], -_z[2], 0,
            0, 0, 0, 1
        );
        auto real = tanf(angle/2);
        this->fx = real / (width/2);
        this->fy = real / (height/2);
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f cameraview{(point.x()-width/2)*fx, (point.y()-height/2)*fy, -1};
        return Ray(center, Transform::transformDirection(R, cameraview));
    }

protected:
    Matrix4f R; // rotation
    float fx, fy;
};

#endif //CAMERA_H
