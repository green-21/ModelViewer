

#include <iostream>
#include <memory>
#include <vector>

#include "MessageHandler.h"
#include "WindowsWindow.h"

#include "GraphicsDevice.h"
#include "GraphicsPipelineStateObject.h"
#include "Renderer.h"
#include "ShaderCompiler.h"

constexpr int width = 800, height = 600;
bool quitRequest = 0;
WindowsWindow window;
std::shared_ptr<MessageHandler> msgHandler;
std::unique_ptr<GraphicsDevice> device;
std::unique_ptr<Renderer> renderer;
GraphicsPipelineStateObject pso;

GraphicsBuffer triangle;

void createPSO();
void init();
void initBuffer();

int main() {
    init();

    window.Show();
    while (!quitRequest) {
        window.PumpMessage();
        renderer->ClearScreen();
        renderer->Draw(triangle);
        renderer->Present();
    }

    return 0;
}

void init() {
    msgHandler = std::make_shared<MessageHandler>(quitRequest);
    window.Init(msgHandler, width, height);

    device = std::make_unique<GraphicsDevice>();
    renderer = device->InitAndCreateRenderer(window.GetHandle(), width, height);
    initBuffer();
    createPSO();
    renderer->SetViewport(width, height);
    renderer->SetPipelineState(pso);
}

void initBuffer() {
    { // vertexInput
        std::vector<Vertex> vertices = {
            {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        };

        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(Vertex);

        triangle = device->CreateGraphicsBuffer(desc, vertices.data());
    }
}
void createPSO() {
    { // vertexShader & InputLayout

        D3DBlob blob = ShaderCompiler::Compile(L"basicVS.hlsl", "vs_5_0");
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

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