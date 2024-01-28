#pragma once

#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "d3d11wrapper.h"

// Imgui
class UI {
public:
    UI() : transform(0.0f), rotation(0.0f), scale(1.0f) {}
    int Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context);
    void Update();
    void Draw();

    Vector3 GetTransform() { return transform; }
    Vector3 GetRotation() { return rotation; }
    Vector3 Getscale() { return scale; }

private:
    Vector3 transform;
    Vector3 rotation;
    Vector3 scale;
};