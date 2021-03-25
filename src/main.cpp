#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    auto scene = SceneParser(inputFile.c_str());
    Image img(scene.getCamera()->getWidth(), scene.getCamera()->getHeight());
    for (int x = 0; x < scene.getCamera()->getWidth(); ++x)
        for (int y = 0; y < scene.getCamera()->getHeight(); ++y) {
            Ray ray = scene.getCamera()->generateRay(Vector2f(x+0.5, y+0.5)); // pixel center
            Hit hit;
            if ( scene.getGroup()->intersect(ray, hit, 0) ) {
                Vector3f color = Vector3f::ZERO;
                for (int li = 0; li < scene.getNumLights(); ++li) {
                    Light* light = scene.getLight(li);
                    Vector3f L, lightColor;
                    light->getIllumination(ray.pointAtParameter(hit.getT()), L, lightColor);
                    color += hit.getMaterial()->Shade(ray, hit, L, lightColor);
                }
                img.SetPixel(x, y, color);
            } else {
                img.SetPixel(x, y, scene.getBackgroundColor());
            }
        }
//    img.FlipHorizontal();
    img.SaveImage(argv[2]);

    return 0;
}

