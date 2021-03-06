#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Vector2f.h"
#include "Vector3f.h"


class Mesh : public Object3D {

public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v; // nodes
    std::vector<TriangleIndex> t; // which 3 nodes index above form a triangle
    std::vector<Vector3f> n; // use computeNormal below after init triangle, calc their normal vector
    bool intersect(const Ray &ray, Hit &hit, float tmin) override;

private:

    // Normal can be used for light estimation
    void computeNormal();
};

#endif
