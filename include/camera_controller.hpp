#pragma once

#include <vecmath.h>
#include <camera.hpp>

class CameraController { // TODO how it works

public:
    typedef enum { NONE, LEFT, MIDDLE, RIGHT } Button;

    // Call from whatever UI toolkit
    void mouseClick(Button button, int x, int y);
    void mouseDrag(int x, int y);
    void mouseRelease(int x, int y);

    CameraController(PerspectiveCamera* cam, float centerDistance);

protected:

    // Camera
    PerspectiveCamera* mpCamera;
    void setFromCamera();
    void applyToCamera();

    // States
    int     mStartClick[2];
    Button  mButtonState;

    // For rotation
    Matrix3f mStartRot;
    Matrix3f mCurrentRot;

    // For translation
    float   mTanPerspective;
    int     mViewport[4];
    Vector3f mStartCenter;
    Vector3f mCurrentCenter;

    // For zoom
    float   mStartDistance;
    float   mCurrentDistance;

    void arcBallRotation(int x, int y);
    void planeTranslation(int x, int y);
    void distanceZoom(int x, int y);

};