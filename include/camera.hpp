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
        this->c = center;
        this->_z = direction.normalized();
        this->y = up.normalized();
        this->x = Vector3f::cross(this->_z, y).normalized();
        this->y = Vector3f::cross(this->x, this->_z);
        this->R = Matrix3f(x, y, -_z);
        this->resize(imgW, imgH);
    }
    virtual ~Camera() = default;

    virtual Ray generateRay(const Vector2f &point) = 0; // Generate rays for each screen-space coordinate

    virtual void setupGLMatrix() {
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(
            c.x(), c.y(), c.z(),   // Position
            c.x() + _z.x(), c.y() + _z.y(), c.z() + _z.z(),   // Any point on LookAt direction
            y.x(), y.y(), y.z()    // Up direction
        );
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getCx() const { return cx; }
    float getCy() const { return cy; }

    void setCenter(const Vector3f& center) { this->c= center; }
    Vector3f getCenter() const { return this->c; }

    void setRotation(const Matrix3f& mat) { 
        this->x = mat.getCol(0);
        this->y = mat.getCol(1);
        this->_z = -mat.getCol(2);
        this->R = Matrix3f(x, y, -_z);
    }

    Matrix3f getRotation() const {
        return this->R;
    }

    virtual void resize(int w, int h) {
        width = w; height = h;
        cx = w/ 2., cy = h/ 2.;
    }

protected:
    Vector3f c;
    Vector3f x, y, _z; // X, Y, -Z
    Matrix3f R; // rotation

    int width, height;
    float cx, cy;
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
        return Ray(c, (this->getRotation() * cameraview).normalized());
    }

    float getFovy() const { return angle/M_PI*180; }

    void setupGLMatrix() override {
        Camera::setupGLMatrix(); // base class func
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(getFovy(), cx / cy, 0.01, 100.0); // angle, w/h ratio, zNear, zFar
    }

    void resize(int w, int h) override { 
        Camera::resize(w, h);
        gen_perspective();
    }

    void gen_perspective() {
        auto real = tanf(angle/2.0f);
        this->fx = real / cx;
        this->fy = real / cy;
    }

protected:
    float angle, fx, fy;
};

#endif //CAMERA_H
