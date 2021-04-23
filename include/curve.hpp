#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
    CurvePoint(const Vector3f& V, const Vector3f& T): V(V), T(T) {}
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
    int n;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {
        n = controls.size() - 1; // 0..n: size()=n+1
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();

        std::vector<Vector3f> deltas;
        for (int i = 0; i < n; ++i)
            deltas.emplace_back(n*(controls[i+1]-controls[i]));

        float d = 1./resolution;
        for (int l = 0; l <= resolution; ++l) {
            float t = d * l;
            data.emplace_back(deCasteljau(controls, t), deCasteljau(deltas, t));
        }
    }

    Vector3f deCasteljau(std::vector<Vector3f> P, float t) {
        int n = P.size()-1;
        for (int i = n-1; i >= 0; --i) {
            for (int j = 0; j <= i; ++j) {
                P[j] = t * P[j+1] + (1 - t) * P[j];
            }
        }
        return P[0];
    }

protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points, int K=4) : Curve(points), K(K) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }
    
    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        
        float d = 1./(n+K)/resolution;
        std::vector<Vector3f> deltas;
        deltas.emplace_back(); // deltas[0] placeholder
        for (int i = 1; i <= n; ++i)
            deltas.emplace_back((K-1)*(controls[i]-controls[i-1])/((K-1)*d));

        for (int ti = K-1; ti <= n; ++ti) {
            for (int l = ti*resolution; l <= (ti+1)*resolution; ++l) {
                float t = d * l;
                data.emplace_back(deBoor(controls, ti, K, t, d*resolution), deBoor(deltas, ti, K-1, t, d*resolution));
            }
        }
    }

    Vector3f deBoor(std::vector<Vector3f>& P, int i, int k, float t, float d) {
        std::vector<Vector3f> p;
        for (int j = i-k+1; j <= i; ++j) p.emplace_back(P[j]);
        for (int r = 1; r <= k-1; ++r) {
            for (int j = k-1; j >= r; --j) {
                auto ti = i-k+1+j;
                p[j] = (t - ti*d) / ((k-r)*d) * p[j] + ((ti+k-r)*d - t) / ((k-r)*d) * p[j-1];
            }
        }
        return p[k-1];
    }

protected:
    int K;
};

#endif // CURVE_HPP