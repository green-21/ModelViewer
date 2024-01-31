#pragma once

#include "d3d11wrapper.h"

class Camera {
public:
    Camera(int width, int height);
    
    const Vector3& GetEyePos() { return eyePos; }
    const Vector3& GetEyeAt() { return eyeAt; }
    const Vector3& GetUpVector() { return upVector; }

    Matrix GetViewMatrix();
    Matrix GetProjectionMatrix();
    void SetAspect(int width, int height);
    void Move(Vector3 q, float dt, float speed);


private:
    Vector3 eyePos;
    Vector3 eyeAt;
    Vector3 upVector;
    float fovY;
    float nearZ;
    float farZ;
    float aspectRatio;
    float distance;
};