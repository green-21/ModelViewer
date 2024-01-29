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
    loadBox();
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

     for (auto &duck : duckModel) {
         auto &matrices = duck.transformationMatrix;
          matrices.model = Matrix::CreateScale(ui.Getscale()) *
                             Matrix::CreateTranslation(ui.GetTransform()) *
                             Matrix::CreateRotationY(ui.GetRotation().y) *
                             Matrix::CreateRotationX(ui.GetRotation().x) *
                             Matrix::CreateRotationZ(ui.GetRotation().z);

         matrices.view = camera.GetViewMatrix();
         matrices.projection = camera.GetProjectionMatrix();

         matrices.Transpose();
         renderer->UpdateBuffer(duck.transformationBuffer, matrices);
    }
}

void MainApp::Draw() {
    renderer->ClearScreen();
    renderer->DrawIndexed(boxModel);
     for (auto &duck : duckModel) {
         renderer->DrawIndexed(duck);
     }
    ui.Draw();
}

Model MainApp::convertMeshToModel(Mesh &mesh) {
    Model result;

    { // vertexBuffer
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(Vertex) * mesh.vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(Vertex);

        result.vertices =
            device->CreateGraphicsBuffer(desc, mesh.vertices.data());
    }

    { // indexBuffer
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
        desc.ByteWidth = UINT(sizeof(uint32_t) * mesh.indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(uint32_t);

        result.indexCount = mesh.indices.size();
        result.indices =
            device->CreateGraphicsBuffer(desc, mesh.indices.data());
    }

    if (!mesh.texturePath.empty()) { // texture
        Image image = FileIO::ReadImage(mesh.texturePath);

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

        result.texture.buffer = device->CreateTextureBuffer2D(
            desc, image.GetData(), image.GetPitch());
        result.texture.view =
            device->CreateShaderResourceView(result.texture.buffer);
    }

    { // transformantion
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = UINT(sizeof(boxModel.transformationMatrix));
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.StructureByteStride = sizeof(Vertex);
        result.transformationBuffer =
            device->CreateGraphicsBuffer(desc, &boxModel.transformationMatrix);
    }

    return result;
}

void MainApp::loadBox() {
    Mesh box;
    box.vertices = {
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
    box.indices = {0,  1,  3,  1,  2,  3,  4,  7,  6,  4,  6,  5,
                   8,  11, 9,  11, 10, 9,  12, 14, 15, 12, 13, 14,
                   16, 19, 18, 16, 17, 19, 20, 22, 23, 20, 23, 21};
    box.texturePath = "resource\\texture\\crate2_diffuse.png";

    boxModel = convertMeshToModel(box);
}

void MainApp::loadModel() {
    ModelLoader loader;
    auto meshes =
         loader.LoadModel("resource\\gltf\\duck\\Duck.gltf");
        // loader.LoadModel("resource\\gltf\\BarramundiFish\\BarramundiFish.gltf");

    for (auto &mesh : meshes) {
        duckModel.push_back(convertMeshToModel(mesh));
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
