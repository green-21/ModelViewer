#pragma once

#include "Model.h"
#include "d3d11wrapper.h"

class Camera {
public:
    Camera(int width, int height);

    const Vector3 &GetPos() { return pos; }
    const Vector3 &GetTargetPoint() { return targetPoint; }
    const Vector3 &GetUpVector() { return up; }

    Matrix GetViewMatrix() { return transformMatrix.view; }
    Matrix GetProjectionMatrix() { return transformMatrix.projection; }
    CameraTransformationMatrix GetTransformMatrix() { return transformMatrix; }
    float GetDistanceFromTarget() { return distance; }
    void SetAspect(int width, int height);
    void Move(Vector3 q, float dt);
    void ZoomIn();
    void ZoomOut();

private:
    void calculateMatrix();

private:
    Vector3 pos{0.0f, 2.0f, -5.0f};
    Vector3 targetPoint{0.0f, 0.0f, 0.0f};
    Vector3 up{0.0f, 1.0f, 0.0f};
    Vector3 right{1.0f, 0.0f, 0.0f};
    float fovY = 70.0f;
    float nearZ = 0.01f;
    float farZ = 1000.0f;
    float aspectRatio;
    float distance;
    float zoomSpeed = 0.1f;
    float orbitSpeed = 5.0f;
    float minZoomDist = 0.1f;
    float maxZoomDist = 100.0f;
    float yDir = -1.0f;
    CameraTransformationMatrix transformMatrix;
};