#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "transform.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->_z = direction.normalized();
        this->y = up.normalized();
        this->x = Vector3f::cross(this->_z, y).normalized();
        this->y = Vector3f::cross(this->x, this->_z);
        this->R = Matrix3f(x, y, -_z);
        this->width = imgW;
        this->height = imgH;
    }
    virtual ~Camera() = default;

    virtual Ray generateRay(const Vector2f &point) = 0; // Generate rays for each screen-space coordinate

    virtual void setupGLMatrix() { // TODO what
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(center.x(), center.y(), center.z(),   // Position
                  center.x() + _z.x(), center.y() + _z.y(), center.z() + _z.z(),   // LookAt
                  y.x(), y.y(), y.z());    // Up direction
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setCenter(const Vector3f& pos) {
        this->center = pos;
    }
    Vector3f getCenter() const {
        return this->center;
    }

    void setRotation(const Matrix3f& mat) { // TODO I modified
        this->x = mat.getCol(0);
        this->y = mat.getCol(1);
        this->_z = -mat.getCol(2);
        this->R = Matrix3f(x, y, -_z);
    }
    Matrix3f getRotation() const { // TODO I modified
        return this->R;
    }

    virtual void resize(int w, int h) {
        width = w; height = h;
    }

protected:
    Vector3f center;
    Vector3f x, y, _z; // X, Y, -Z
    Matrix3f R; // rotation

    int width;
    int height;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH, float angle)
        : Camera(center, direction, up, imgW, imgH) {
        this->angle = angle;
        gen_perspective();
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f cameraview((point.x()-cx)*fx, (point.y()-cy)*fy, -1);
        return Ray(center, (this->R * cameraview).normalized());
    }

    void setupGLMatrix() override { // TODO what
        Camera::setupGLMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(angle/M_PI*180, cx / cy, 0.01, 100.0);
    }

    void resize(int w, int h) override { 
        width = w; height = h;
        gen_perspective();
    }

    void gen_perspective() { // TODO I modified
        auto real = tanf(angle/2.0f);
        this->cx = width / 2.0f;
        this->cy = height / 2.0f;
        this->fx = real / cx;
        this->fy = real / cy;
    }

protected:
    float angle;
    float cx, cy, fx, fy;
};

#endif //CAMERA_H
