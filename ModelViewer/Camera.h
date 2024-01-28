#pragma once

#include "d3d11wrapper.h"

class Camera {
public:
    Camera(int width, int height);
    
    Vector3 GetEyePos() { return eyePos; }
    Vector3 GetEyeDir() { return eyeDir; }
    Vector3 GetUpVector() { return upVector; }

    Matrix GetViewMatrix();
    Matrix GetProjectionMatrix();
    void SetAspect(int width, int height);


private:
    Vector3 eyePos;
    Vector3 eyeDir;
    Vector3 upVector;
    float fovY;
    float nearZ;
    float farZ;
    float aspectRatio;
};