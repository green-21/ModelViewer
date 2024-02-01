#include "Camera.h"

#include <algorithm>
#include <iostream>

Camera::Camera(int width, int height) : distance(pos.Length()) {
    Vector3 dir = (targetPoint - pos);
    // up = dir.Cross(right);
    // up.Normalize();

    SetAspect(width, height);
}

Matrix Camera::GetViewMatrix() {
    return DirectX::XMMatrixLookAtLH(pos, targetPoint, up);
}

Matrix Camera::GetProjectionMatrix() {
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovY),
                                             aspectRatio, nearZ, farZ);
}

void Camera::SetAspect(int width, int height) {
    aspectRatio = float(width) / float(height);
}

float ClampAngle(float angle, float min, float max) {
    if (angle < -360.0f)
        angle += 360.0f;

    if (angle > 360.0f)
        angle -= 360.0f;
    return std::clamp(angle, min, max);
}

// dir is currentNdcPos - prevNdcPos
void Camera::Move(Vector3 delta, float dt) {

    delta = delta * dt * orbitSpeed;

    std::cout << pos.Dot(up) << std::endl;

    Matrix yawMatrix = Matrix();
    if (abs(pos.Dot(up) - pos.Length()) >= 1.0f) {
        yawMatrix = Matrix::CreateFromQuaternion(
            Quaternion::CreateFromAxisAngle(up, -delta.x));
    }

    Matrix pitchMatrix = Matrix::CreateFromQuaternion(
        Quaternion::CreateFromAxisAngle(right, -delta.y));
    pos = Vector3::Transform(pos, pitchMatrix * yawMatrix);

    right = Vector3::Transform(right, yawMatrix);
    // up = Vector3::Transform(up, pitchMatrix);
}

void Camera::ZoomIn() {
    if (pos.Length() >= minZoomDist)
        return;

    pos -= pos * zoomSpeed;
}

void Camera::ZoomOut() {
    if (pos.Length() <= maxZoomDist)
        return;

    pos += pos * zoomSpeed;
}
