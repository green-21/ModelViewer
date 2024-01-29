#include "MainApp.h"
#include "FileIO.h"

MainApp::MainApp(int width, int height)
    : ApplicationBase(width, height), camera(width, height) {}

int MainApp::Init() {
    if (ui.Init(screenWidth, screenHeight, window.GetHandle(), device->Get(),
                renderer->GetContext())) {
        return -1;
    }

    createPSO();
    renderer->SetViewport(screenWidth, screenHeight);
    renderer->SetPipelineState(pso);
    return 0;
}

int MainApp::Load() {
    //loadBox();
    loadModel();
    return 0;
}

void MainApp::Update() {
    ui.Update();
    //auto &matrices = boxModel.transformationMatrix;
    //matrices.model = Matrix::CreateScale(ui.Getscale()) *
    //                 Matrix::CreateTranslation(ui.GetTransform()) *
    //                 Matrix::CreateRotationY(ui.GetRotation().y) *
    //                 Matrix::CreateRotationX(ui.GetRotation().x) *
    //                 Matrix::CreateRotationZ(ui.GetRotation().z);

    //matrices.view = camera.GetViewMatrix();
    //matrices.projection = camera.GetProjectionMatrix();

    //matrices.Transpose();
    //renderer->UpdateBuffer(boxModel.transformationBuffer, matrices);

        auto &matrices = duckModel.transformationMatrix;
    matrices.model = Matrix::CreateScale(ui.Getscale()) *
                     Matrix::CreateTranslation(ui.GetTransform()) *
                     Matrix::CreateRotationY(ui.GetRotation().y) *
                     Matrix::CreateRotationX(ui.GetRotation().x) *
                     Matrix::CreateRotationZ(ui.GetRotation().z);

    matrices.view = camera.GetViewMatrix();
    matrices.projection = camera.GetProjectionMatrix();

    matrices.Transpose();
    renderer->UpdateBuffer(duckModel.transformationBuffer, matrices);
}

void MainApp::Draw() {
    renderer->ClearScreen();
    //renderer->DrawIndexed(boxModel);
    renderer->DrawIndexed(duckModel);
    ui.Draw();
}

void MainApp::loadBox() {
    MeshData cubeMeshData;
    cubeMeshData.vertices = {
        // front
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // back
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // top
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

        // bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

        // right
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    cubeMeshData.indices = {0,  1,  3,  1,  2,  3,  4,  7,  6,  4,  6,  5,
                   8,  11, 9,  11, 10, 9,  12, 14, 15, 12, 13, 14,
                   16, 19, 18, 16, 17, 19, 20, 22, 23, 20, 23, 21};

    cubeMeshData.texturePath = "resource\\texture\\crate2_diffuse.png";

    resourceManager->CreateModelMeshAndTexture("cube", cubeMeshData);
    boxModel = resourceManager->CreateModelFromStroageData("cube");
}

void MainApp::loadModel() {
    resourceManager->LoadModelFromFile("duck",  "resource\\gltf\\duck\\Duck.gltf");
        // loader.LoadModel("resource\\gltf\\FlightHelmet\\FlightHelmet.gltf");
         //loader.LoadModel("resource\\gltf\\SciFiHelmet\\SciFiHelmet.gltf");
        //loader.LoadModel("resource\\gltf\\BarramundiFish\\BarramundiFish.gltf");

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

        pso.vertexShader = device->CreateVertexShader(blob);
        pso.inputLayout = device->CreateInputLayout(blob, inputElements);
    }

    { // pixelShader
        D3DBlob blob = ShaderCompiler::Compile(L"basicPS.hlsl", "ps_5_0");

        pso.pixelShader = device->CreatePixelShader(blob);
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
        pso.samplerState = device->CreateSamplerState(desc);
    }

    { // RasterizerState
        D3D11_RASTERIZER_DESC desc{};
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        desc.FrontCounterClockwise = false;
        desc.DepthClipEnable = true;
        desc.MultisampleEnable = true;

        pso.rasterizerState = device->CreateRasterizerState(desc);
    }

    {
        D3D11_DEPTH_STENCIL_DESC desc{};
        desc.DepthEnable = true;
        desc.DepthWriteMask =
            D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        pso.depthStencilState = device->CreateDepthStencilState(desc);
    }

    pso.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
