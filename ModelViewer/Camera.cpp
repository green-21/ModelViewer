#include "Camera.h"

#include <algorithm>
#include <iostream>

Camera::Camera(int width, int height) : distance(pos.Length()) {
    SetAspect(width, height);
    calculateMatrix();
}

void Camera::SetAspect(int width, int height) {
    aspectRatio = float(width) / float(height);
}

// dir is currentNdcPos - prevNdcPos
void Camera::Move(Vector3 delta, float dt) {
    const Vector3 yAxis{0.0f, 1.0f, 0.0f};

    delta = delta * dt * orbitSpeed;

    if (yDir * up.Dot(yAxis) > 0) {
        yDir = -yDir;
    }

    Matrix yawMatrix = Matrix::CreateFromQuaternion(
        Quaternion::CreateFromAxisAngle(yAxis, yDir * delta.x));

    Matrix pitchMatrix = Matrix::CreateFromQuaternion(
        Quaternion::CreateFromAxisAngle(right, -delta.y));
    pos = Vector3::Transform(pos, pitchMatrix * yawMatrix);

    right = Vector3::Transform(right, yawMatrix);

    up = right.Cross(pos);
    up.Normalize();

    calculateMatrix();
}

void Camera::ZoomIn() {
    if (pos.Length() <= minZoomDist)
        return;

    pos -= pos * zoomSpeed;
    calculateMatrix();
}

void Camera::ZoomOut() {
    if (pos.Length() >= maxZoomDist)
        return;

    pos += pos * zoomSpeed;
    calculateMatrix();
}

void Camera::calculateMatrix() {
    transformMatrix.view = DirectX::XMMatrixLookAtLH(pos, targetPoint, up);
    transformMatrix.projection = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(fovY), aspectRatio, nearZ, farZ);
    transformMatrix.invProjection = transformMatrix.projection.Invert();
    // transformMatrix.invProjection.Invert();

    transformMatrix.Transpose();
    transformMatrix.cameraPos = pos;
}
