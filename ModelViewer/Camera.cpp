#include "Camera.h"


Camera::Camera(int width, int height)
    : eyePos(Vector3(0.0f, 0.0f, -2.0f)), eyeDir(Vector3(0.0f, 0.0f, 1.0f)),
      upVector(Vector3(0.0f, 1.0f, 0.0f)), fovY(70.0f), nearZ(0.01f),
      farZ(100.0f) {
    SetAspect(width, height);
}

Matrix Camera::GetViewMatrix() {
    return DirectX::XMMatrixLookToLH(eyePos, eyeDir, upVector);
}

Matrix Camera::GetProjectionMatrix() {
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovY),
                                             aspectRatio, nearZ, farZ);
}

void Camera::SetAspect(int width, int height) {
    aspectRatio = float(width) / float(height);
}
