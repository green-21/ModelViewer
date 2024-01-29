#include "UI.h"

int UI::Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(width), float(height));
    //ImGui::StyleColorsLight();

    if (!ImGui_ImplDX11_Init(device, context)) {
        return -1;
    }

    if (!ImGui_ImplWin32_Init(hWnd)) {
        return -1;
    }
    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 16.0f, NULL,
    //                             io.Fonts->GetGlyphRangesKorean());
    return 0;
}

void UI::Update() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::Begin("ui");
    ImGui::Text("[FPS] %.1f", ImGui::GetIO().Framerate);
    ImGui::SliderFloat3(u8"pos", &transform.x, -1.0f, 1.0f);
    ImGui::SliderFloat3(u8"rotate", &rotation.x, -3.14f, 3.14f);
    ImGui::SliderFloat(u8"scale", &scale, 0.0f, 3.0f);
    ImGui::End();
    ImGui::Render();
}

void UI::Draw() { ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); }
