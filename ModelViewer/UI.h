#pragma once

#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <string>

#include "ResourceManager.h"
#include "d3d11wrapper.h"

// Imgui
class UI {
public:
    UI(std::vector<ModelObject> &models) : isRenderAxis(true), models(models) {}
    int Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context,
             std::shared_ptr<ResourceManager> resourceManager);
    void Update();
    void Draw();

    int IsSelectedModel(int idx) { return selectedModel == idx; }
    void UpdateCameraPos(const Vector3 &pos) { cameraPos = pos; }
    bool IsRenderAxis() { return isRenderAxis; }
    bool IsUsingUI() { return isUsingUI; }

private:
    void openFile();
    void setUIUsingState(bool state) { isUsingUI = state; }
    void updateUIUsingState() {
        isUsingUI =
            isUsingUI || ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();
    }

private:
    std::shared_ptr<ResourceManager> resourceManager;
    std::vector<ModelObject> &models;

    Vector3 cameraPos;
    bool isRenderAxis = true;
    bool isUsingUI = false;
    char filepath[512];
    int selectedModel = -1;

    bool *p_open = nullptr;
};