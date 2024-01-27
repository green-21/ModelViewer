#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <Windows.h>

// Imgui
class UI {
public:
    static int Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context);
    static void Update();
    static void Draw();
};