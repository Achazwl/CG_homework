#ifndef GROUP_H
#define GROUP_H

#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

public:

    Group() : objs(0) { }
    explicit Group (int num_objects) : objs(num_objects) {}
    ~Group() override { }

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        bool result = false;
        for (auto obj : objs) {
            result |= obj->intersect(ray, hit, tmin);
        }
        return result;
    }

    void drawGL() override {
        for (auto obj : objs) {
            obj->drawGL();
        }
    }

    void addObject(int index, Object3D *obj) {
        objs[index] = obj;
    }

    int getGroupSize() const { return objs.size(); }

private:

    std::vector<Object3D*> objs;
};

#endif
	
