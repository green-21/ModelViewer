#include "UI.h"

int UI::Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(width), float(height));
    ImGui::StyleColorsLight();

    if (!ImGui_ImplDX11_Init(device, context)) {
        return -1;
    }

    if (!ImGui_ImplWin32_Init(hWnd)) {
        return -1;
    }

    return 0;
}

void UI::Update() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("example");

    ImGui::Text("FPS:%.1f", ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
}

void UI::Draw() { ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); }
