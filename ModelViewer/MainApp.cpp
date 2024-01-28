#include "MainApp.h"
#include "FileIO.h"

MainApp::MainApp(int width, int height) : ApplicationBase(width, height), camera(width, height) {}

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
    loadBox();

    return 0;
}

void MainApp::Update() {
    ui.Update();
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

void MainApp::Draw() {
    renderer->ClearScreen();
    renderer->DrawIndexed(model);
    ui.Draw();
}

void MainApp::loadBox() {
    Mesh box;
    box.vertices = {
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };
    box.indices = {0, 1, 3, 1, 2, 3};
    box.texturePath = "crate2_diffuse.png";

    { // vertexBuffer
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(Vertex) * box.vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(Vertex);

        model.vertices =
            device->CreateGraphicsBuffer(desc, box.vertices.data());
    }

    { // indexBuffer

        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
        desc.ByteWidth = UINT(sizeof(uint32_t) * box.indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(uint32_t);

        model.indexCount = box.indices.size();
        model.indices = device->CreateGraphicsBuffer(desc, box.indices.data());
    }

    { // texture
        Image image = FileIO::ReadImage(box.texturePath);

        D3D11_TEXTURE2D_DESC desc{};
        desc.ArraySize = 1;
        desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Width = image.GetWidth();
        desc.Height = image.GetHeight();
        desc.MipLevels = 1;
        desc.MiscFlags = 0;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;

        model.texture.buffer = device->CreateTextureBuffer2D(
            desc, image.GetData(), image.GetPitch());
        model.texture.view =
            device->CreateShaderResourceView(model.texture.buffer);
    }

    { // transformantion
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = UINT(sizeof(model.transformationMatrix));
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.StructureByteStride = sizeof(Vertex);
        model.transformationBuffer =
            device->CreateGraphicsBuffer(desc, &model.transformationMatrix);
    }
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

    pso.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
