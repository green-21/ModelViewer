#include "MainApp.h"
#include "FileIO.h"
#include "MeshGenerator.h"

MainApp::MainApp(int width, int height)
    : ApplicationBase(width, height), camera(width, height) {}

int MainApp::Init() {
    if (ui.Init(screenWidth, screenHeight, window.GetHandle(), device->Get(),
                renderer->GetContext())) {
        return -1;
    }

    createPSO();
    renderer->SetViewport(screenWidth, screenHeight);
    renderer->SetClearColor(Vector3(0.25f));
    renderer->SetCameraMatrix(
        device->CreateConstantBuffer(camera.GetTransformMatrix()));

    return 0;
}

void MainApp::createPSO() {
    { // vertexShader & InputLayout

        D3DBlob blob = ShaderCompiler::Compile(L"basicVS.hlsl", "vs_5_0");
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
             D3D11_INPUT_PER_VERTEX_DATA, 0}};

        defaultPSO.vertexShader = device->CreateVertexShader(blob);
        defaultPSO.inputLayout = device->CreateInputLayout(blob, inputElements);
    }

    { // pixelShader
        D3DBlob blob = ShaderCompiler::Compile(L"basicPS.hlsl", "ps_5_0");

        defaultPSO.pixelShader = device->CreatePixelShader(blob);
    }

    { // SamplerState
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;
        defaultPSO.samplerState = device->CreateSamplerState(desc);
    }

    { // RasterizerState
        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        desc.FrontCounterClockwise = false;
        desc.DepthClipEnable = true;
        desc.MultisampleEnable = true;

        defaultPSO.rasterizerState = device->CreateRasterizerState(desc);
    }

    {
        D3D11_DEPTH_STENCIL_DESC desc{};
        desc.DepthEnable = true;
        desc.DepthWriteMask =
            D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        defaultPSO.depthStencilState = device->CreateDepthStencilState(desc);
    }
    defaultPSO.vertexShaderConstantBuffers.push_back(
        device->CreateConstantBuffer(ModelTransformationMatrix()));
    defaultPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    axisPSO = defaultPSO;
    { // RasterizerState
        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        desc.FrontCounterClockwise = false;
        desc.DepthClipEnable = true;
        desc.MultisampleEnable = true;

        axisPSO.rasterizerState = device->CreateRasterizerState(desc);
    }
    {
        // vertex
        // D3DBlob blob = ShaderCompiler::Compile(L"basicVS.hlsl", "vs_5_0");
        // axisPSO.vertexShader = device->CreateVertexShader(blob);
    }

    { // pixelShader
        D3DBlob blob = ShaderCompiler::Compile(L"gridPS.hlsl", "ps_5_0");
        axisPSO.pixelShader = device->CreatePixelShader(blob);
    }

    { // blend
        D3D11_BLEND_DESC desc{};
        desc.IndependentBlendEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
        desc.RenderTarget[0].RenderTargetWriteMask =
            D3D11_COLOR_WRITE_ENABLE_ALL;

        axisPSO.blendState = device->CreateBlendState(desc);
    }
    axisPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    GraphicsPipelineStateObject postPSO = defaultPSO;
    { // vertexShader & InputLayout

        D3DBlob blob = ShaderCompiler::Compile(L"pathThroughVS.hlsl", "vs_5_0");
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
             D3D11_INPUT_PER_VERTEX_DATA, 0}};

        postPSO.vertexShader = device->CreateVertexShader(blob);
        postPSO.inputLayout = device->CreateInputLayout(blob, inputElements);
    }

    { // pixelShader
        D3DBlob blob = ShaderCompiler::Compile(L"postPS.hlsl", "ps_5_0");
        postPSO.pixelShader = device->CreatePixelShader(blob);
    }

    { // RasterizerState
        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        desc.FrontCounterClockwise = false;
        desc.DepthClipEnable = false;
        desc.MultisampleEnable = true;

        postPSO.rasterizerState = device->CreateRasterizerState(desc);
    }
    renderer->SetPostRenderPSO(postPSO);
}

int MainApp::Load() {
    loadBox();
    loadModel();
    loadGrid();
    return 0;
}
void MainApp::loadBox() {
    resourceManager->CreateMesh("cubeMesh", MeshGenerator::GenerateCube());
    resourceManager->LoadTextureFromFile("boxTexture", "resource\\texture\\crate2_diffuse.png");
    boxModel = resourceManager->CreateModelFromStorage("cube", {"cubeMesh"},
                                                       {"boxTexture"});
}

void MainApp::loadGrid() {
    resourceManager->CreateMesh("gridMesh",
                                MeshGenerator::GenerateXZSquare(500.0f));
    gridModel = resourceManager->CreateModelFromStorage("grid", {"gridMesh"}, {});
}

void MainApp::loadModel() {
    resourceManager->LoadModelFromFile("duck",
                                       "resource\\gltf\\duck\\Duck.gltf");
    // "resource\\gltf\\FlightHelmet\\FlightHelmet.gltf"
    // "resource\\gltf\\SciFiHelmet\\SciFiHelmet.gltf"
    // "resource\\gltf\\BarramundiFish\\BarramundiFish.gltf"

    duckModel = resourceManager->UseModel("duck");
}

void MainApp::Update(float dt) {
    if (msgHandler->IsKeyPress(KeyCode::ESC)) {
        msgHandler->OnQuit();
    }

    ui.UpdateCameraPos(camera.GetPos());
    ui.Update();

    cameraUpdate(dt);
    renderer->UpdateCameraMatrix(camera.GetTransformMatrix());

    //defaultUpdate(boxModel);
    defaultUpdate(duckModel);
    //defaultUpdate(gridModel);
}

void MainApp::Draw() {
    renderer->ClearScreen();

    renderer->SetPipelineState(defaultPSO);
    //renderer->DrawIndexed(boxModel);
    renderer->DrawIndexed(duckModel);

    if (ui.IsRenderAxis()) {
        renderer->SetPipelineState(axisPSO);
        renderer->DrawIndexed(gridModel);
    }
    renderer->PostProcess();
    ui.Draw();
}

void MainApp::defaultUpdate(Model &model) {
    auto &matrices = model.transformationMatrix;
    matrices.model = Matrix::CreateScale(ui.Getscale()) *
                     Matrix::CreateTranslation(ui.GetTransform()) *
                     Matrix::CreateRotationY(ui.GetRotation().y) *
                     Matrix::CreateRotationX(ui.GetRotation().x) *
                     Matrix::CreateRotationZ(ui.GetRotation().z);

    matrices.Transpose();
}

void printVector(const std::string &text, Vector3 &v) {
    std::cout << "[" << text << "] " << v.x << ", " << v.y << ", " << v.z
              << std::endl;
}

void MainApp::cameraUpdate(float dt) {
    static Vector3 prevCursorPos;

    // wheel
    const int wheel = msgHandler->GetWheelMouse();

    if (wheel > 0) {
        camera.ZoomIn();
    } else if (wheel < 0) {
        camera.ZoomOut();
    }

    // mouse move and rotate
    if (msgHandler->IsLeftMousePress()) {
        Vector3 currentCursorPos(0.0f);
        currentCursorPos.x = float(msgHandler->GetMousePosX()) / screenWidth;
        currentCursorPos.y = float(msgHandler->GetMousePosY()) / screenHeight;

        if (msgHandler->IsLeftMouseDragStart()) {
            msgHandler->OffLeftMouseDragStart();
            prevCursorPos = currentCursorPos;
            return;
        }

        Vector3 delta = (prevCursorPos - currentCursorPos);
        if (delta.Length() >= 1e-5) {
            delta.Normalize();
            camera.Move(delta, dt);
            prevCursorPos = currentCursorPos;
        }
    }
}
