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
    return 0;
}

int MainApp::Load() {
    loadBox();
    loadModel();
    loadGrid();
    return 0;
}

void MainApp::Update() {
    ui.Update();
    defaultUpdate(boxModel);
    defaultUpdate(duckModel);
    defaultUpdate(gridModel);
}

void MainApp::Draw() {
    renderer->ClearScreen();

    if (ui.IsRenderAxis()) {
        renderer->SetPipelineState(axisPSO);
        renderer->DrawIndexed(gridModel);
    }

    renderer->SetPipelineState(defaultPSO);
    renderer->DrawIndexed(boxModel);
    renderer->DrawIndexed(duckModel);
    ui.Draw();
}

void MainApp::loadBox() {
    MeshData cube = MeshGenerator::GenerateCube();
    cube.texturePath = "resource\\texture\\crate2_diffuse.png";

    resourceManager->CreateModelMeshAndTexture("cube", cube);
    boxModel = resourceManager->CreateModelFromStroageData("cube");
}

void MainApp::loadGrid() {
    MeshData gridData = MeshGenerator::GenerateAxisGrid(5000, 100);
    resourceManager->CreateModelMeshAndTexture("grid", gridData);
    gridModel = resourceManager->CreateModelFromStroageData("grid");
}

void MainApp::loadModel() {
    resourceManager->LoadModelFromFile("duck",
                                       "resource\\gltf\\duck\\Duck.gltf");
    // "resource\\gltf\\FlightHelmet\\FlightHelmet.gltf"
    // "resource\\gltf\\SciFiHelmet\\SciFiHelmet.gltf"
    // "resource\\gltf\\BarramundiFish\\BarramundiFish.gltf"

    duckModel = resourceManager->UseModel("duck");
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

    defaultPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    axisPSO = defaultPSO;
    { // pixelShader
        D3DBlob blob = ShaderCompiler::Compile(L"gridPS.hlsl", "ps_5_0");
        axisPSO.pixelShader = device->CreatePixelShader(blob);
    }
    axisPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

void MainApp::defaultUpdate(Model &model) {
    auto &matrices = model.transformationMatrix;
    matrices.model = Matrix::CreateScale(ui.Getscale()) *
                     Matrix::CreateTranslation(ui.GetTransform()) *
                     Matrix::CreateRotationY(ui.GetRotation().y) *
                     Matrix::CreateRotationX(ui.GetRotation().x) *
                     Matrix::CreateRotationZ(ui.GetRotation().z);

    matrices.view = camera.GetViewMatrix();
    matrices.projection = camera.GetProjectionMatrix();

    matrices.Transpose();
    renderer->UpdateBuffer(model.transformationBuffer, matrices);
}
