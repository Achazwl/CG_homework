#ifndef IMAGE_H
#define IMAGE_H

#include <cassert>
#include <vecmath.h>

// Simple image class
class Image {

public:

    Image(int w, int h) {
        width = w;
        height = h;
        data = new Vector3f[width * height];
    }

    ~Image() {
        delete[] data;
    }

    int Width() const {
        return width;
    }

    int Height() const {
        return height;
    }

    bool check(int x, int y) const {
        return (x >= 0 && x < width) && (y >= 0 && y < height);
    }

    const Vector3f &GetPixel(int x, int y) const {
        assert(check(x, y));
        return data[y * width + x];
    }

    void SetAllPixels(const Vector3f &color) {
        for (int i = 0; i < width * height; ++i) {
            data[i] = color;
        }
    }

    void SetPixel(int x, int y, const Vector3f &color) {
        if (check(x, y))
            data[y * width + x] = color;
    }

    void FlipHorizontal() {
        int ys = 0;
        int ye = height - 1;
        while (ye > ys) {
            for (int x = 0; x < width; ++x) {
                Vector3f tmp = data[ys * width + x];
                data[ys * width + x] = data[ye * width + x];
                data[ye * width + x] = tmp;
            }
            --ye; ++ys;
        }
    }

    static Image *LoadPPM(const char *filename);

    void SavePPM(const char *filename) const;

    static Image *LoadTGA(const char *filename);

    void SaveTGA(const char *filename) const;

    int SaveBMP(const char *filename);

    void SaveImage(const char *filename);

private:

    int width;
    int height;
    Vector3f *data;

};

#endif // IMAGE_H
