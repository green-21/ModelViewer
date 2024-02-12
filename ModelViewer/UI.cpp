#include "UI.h"
#include <sstream>
#include <string>

int UI::Init(int width, int height, HWND hWnd, ID3D11Device *device,
             ID3D11DeviceContext *context,
             std::shared_ptr<ResourceManager> resourceManager) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(width), float(height));
    // ImGui::StyleColorsLight();

    if (!ImGui_ImplDX11_Init(device, context)) {
        return -1;
    }

    if (!ImGui_ImplWin32_Init(hWnd)) {
        return -1;
    }
    this->resourceManager = resourceManager;
    // io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 16.0f,
    // NULL,
    //                              io.Fonts->GetGlyphRangesKorean());
    return 0;
}

void UI::Update() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    setUIUsingState(false);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(280.0f, 160.0f));
    ImGui::Begin("ui", p_open,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse);
    updateUIUsingState();

    ImGui::Text("[FPS] %.1f", ImGui::GetIO().Framerate);
    ImGui::Checkbox("axis line", &isRenderAxis);
    ImGui::Text("camera : (%.3f, %.3f, %3f)", cameraPos.x, cameraPos.y,
                cameraPos.z);

    if (selectedModel != -1) {
        auto &model = models[selectedModel];
        ImGui::Text(model.name.c_str());
        ImGui::SliderFloat3("pos", (float *)&model.pos, -10.0f, 10.0f);
        ImGui::SliderFloat("scale", (float *)&model.scale, 0.0, 3.0);
        model.scale = Vector3(model.scale.x);
    }
    ImGui::End();

    /*
    ImGui::SetNextWindowSize(ImVec2(280.0f, 300.0f));
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 0.0f));

    ImGui::Begin("Resources", p_open,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Load Model")) {
        openFile();
    }
    ImGui::SameLine();
    ImGui::Button("Load Texture");

    ImGui::BeginTable("re", 3);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Nodes");
    ImGui::TableSetupColumn("visible");
    ImGui::TableHeadersRow();

    auto modelStorage = resourceManager->GetModelStorage();
    for (auto iter = modelStorage.Begin(); iter != modelStorage.End(); iter++) {
        auto &[name, data] = *iter;

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::Text(name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%d", data.nodes.size());
        bool a;
        ImGui::Checkbox("hi", &a);
    }
    ImGui::EndTable();
    ImGui::End();*/

    ImGui::SetNextWindowPos(ImVec2(1000.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(280.0f, 300.0f));
    ImGui::Begin("Objects", p_open,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse);
    updateUIUsingState();

    if (ImGui::Button("Load Model Object")) {
        openFile();
    }
    if (models.size() == 0) {
        ImGui::PushTextWrapPos(280.0f);
        ImGui::TextDisabled(
            "Not found drawble model Object. Please load model..");
    } else {
        ImGui::BeginTable("ModelTable", 2);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("value");
        ImGui::TableHeadersRow();

        for (int n = 0; n < models.size(); n++) {
            auto &obj = models[n];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable(obj.name.c_str(), selectedModel == n,
                                  ImGuiSelectableFlags_SpanAllColumns)) {
                selectedModel = selectedModel == n ? -1 : n;
            }
            ImGui::TableNextColumn();
            ImGui::Text(" ");
        }
        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::Render();
}

void UI::Draw() { ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); }

void UI::openFile() {
    OPENFILENAME ofn; // 파일 대화 상자 정보를 담을 구조체
    wchar_t wFilePath[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = wFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(wFilePath);
    ofn.lpstrFilter = L"GLTF Files\0*.gltf;*.glb\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 취소했거나 실패했음.
    if (!GetOpenFileName(&ofn))
        return;

    int bufferSize =
        WideCharToMultiByte(CP_UTF8, 0, wFilePath, -1, NULL, 0, NULL, NULL);
    std::string filePath(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wFilePath, -1, &filePath[0], bufferSize,
                        NULL, NULL);

    const size_t start = filePath.find_last_of('\\') + 1;
    const size_t end = filePath.find_last_of('.') - start;
    std::string name = filePath.substr(start, end);
    resourceManager->LoadModelFromFile(name, filePath);
    selectedModel = int(models.size());
    models.push_back({name.c_str(), resourceManager->GetModel(name)});
}
