#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <queue>

struct Point {
    int x, y;
    Point(int x, int y):x(x), y(y) {}
    Point operator + (const Point& rhs) {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator - (const Point& rhs) {
        return Point(x - rhs.x, y - rhs.y);
    }
    Point operator - () {
        return Point(-x, -y);
    }
};
using Vector = Point;

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;

    void draw(Image &img) override {
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB, color.x(), color.y(), color.z());

        Point u(xA, yA), v(xB, yB);
        Vector d = v - u;
        if (std::abs(d.y) <= std::abs(d.x)) { // 偏横向倾斜
            if (u.x > v.x) {
                std::swap(u, v);
                d = -d;
            }
            int y = u.y, e = -d.x, dy = std::abs(d.y);
            for (int x = u.x; x <= v.x; ) {
                img.SetPixel(x, y, color);
                ++x;
                e += 2 * dy;
                if (e >= 0) {
                    y = d.y > 0 ? y + 1 : y - 1;
                    e -= 2 * d.x;
                }
            }
        }
        else { // 偏纵向倾斜
            if (u.y > v.y) {
                std::swap(u, v);
                d = -d;
            }
            int x = u.x, e = -d.y, dx = std::abs(d.x);
            for (int y = u.y; y <= v.y; ) {
                img.SetPixel(x, y, color);
                ++y;
                e += 2 * dx;
                if (e >= 0) {
                    x = d.x > 0 ? x + 1 : x - 1;
                    e -= 2 * d.y;
                }
            }
        }
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;

    void eight(int x, int y, Image& img) {
        img.SetPixel(cx + x, cy + y, color);
        img.SetPixel(cx + x, cy - y, color);
        img.SetPixel(cx - x, cy + y, color);
        img.SetPixel(cx - x, cy - y, color);
        img.SetPixel(cx + y, cy + x, color);
        img.SetPixel(cx + y, cy - x, color);
        img.SetPixel(cx - y, cy + x, color);
        img.SetPixel(cx - y, cy - x, color);
    };

    void draw(Image &img) override {
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius, color.x(), color.y(), color.z());

        int y = radius, d = 5 - 4 * radius;
        for (int x = 0; x <= y; ++x) {
            eight(x, y, img);
            if (d < 0)
                d += 8 * x + 12;
            else {
                d += 8 * (x - y) + 20;
                --y;
            }
        }
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;

    void draw(Image &img) override {
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy, color.x(), color.y(), color.z());

        auto oldcolor = img.GetPixel(cx, cy);
        auto isold = [&oldcolor, &img](const Point& p) bool {
            return img.check(p.x, p.y) && img.GetPixel(p.x, p.y) == oldcolor;
        };
        auto q = std::queue<Point>();
        q.emplace(cx, cy);
        while (!q.empty()) {
            auto u = q.front(); q.pop();
            if (img.GetPixel(u.x, u.y) == color) continue;
            auto left = u, right = u;
            for (; isold(left); --left.x)
                img.SetPixel(left.x, left.y, color);
            ++left.x;
            for (++right.x; isold(right); ++right.x)
                img.SetPixel(right.x, right.y, color);
            --right.x;

            auto expand = [&q, &img, &right, &oldcolor](Point v) {
                if (img.check(v.x, v.y)) {
                    for (; v.x <= right.x; ) {
                        if (img.GetPixel(v.x, v.y) == oldcolor) {
                            q.push(v);
                            for (; v.x <= right.x && img.GetPixel(v.x, v.y) == oldcolor; ++v.x);
                        }
                        else {
                            ++v.x;
                        }
                    }
                }
            };
            expand({left.x, left.y+1});
            expand({left.x, left.y-1});
        }
    }
};
