#include "Camera.h"

#include <iostream>

Camera::Camera(int width, int height)
    : eyePos(Vector3(0.0f, 2.0f, -5.0f)), eyeAt(Vector3(0.0f, 0.0f, 0.0f)),
      fovY(70.0f), nearZ(0.01f), farZ(100.0f), distance(5.0f) {
    Vector3 dir = (eyeAt - eyePos);
    Vector3 right{1.0f, 0.0f, 0.0f};
    upVector = dir.Cross(right);
    upVector.Normalize();

    std::cout << upVector.Dot(dir) << std::endl;
    SetAspect(width, height);
}

Matrix Camera::GetViewMatrix() {
    return DirectX::XMMatrixLookAtLH(eyePos, eyeAt, upVector);
}

Matrix Camera::GetProjectionMatrix() {
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovY),
                                             aspectRatio, nearZ, farZ);
}

void Camera::SetAspect(int width, int height) {
    aspectRatio = float(width) / float(height);
}

void Camera::Move(Vector3 dir, float dt, float speed) {

    // std::cout << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
    eyePos += dir * dt * speed;
    const Vector3 corrDir = eyeAt - eyePos;
    const float factor = 1 - distance / corrDir.Length();
    std::cout << factor << std::endl;
        eyePos += corrDir * factor;
    // std::cout << (eyeAt - eyePos).Length() << std::endl;
}
