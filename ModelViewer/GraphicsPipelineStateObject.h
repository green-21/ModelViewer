#pragma once

#include "d3d11wrapper.h"

struct GraphicsPipelineStateObject {
public:
    void operator=(const GraphicsPipelineStateObject &pso) {
        vertexShader = pso.vertexShader;
        pixelShader = pso.pixelShader;
        rasterizerState = pso.rasterizerState;
        inputLayout = pso.inputLayout;
        samplerState = pso.samplerState;
        primitiveTopology = pso.primitiveTopology;
        depthStencilState = pso.depthStencilState;
    }

public:
    VertexShader vertexShader;
    PixelShader pixelShader;
    RasterizerState rasterizerState;
    SamplerState samplerState;
    InputLayout inputLayout;
    DepthStencilState depthStencilState;
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};