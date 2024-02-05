#pragma once

#include "d3d11wrapper.h"

struct GraphicsPipelineStateObject {
public:
    void operator=(const GraphicsPipelineStateObject &pso) {
        vertexShader = pso.vertexShader;
        pixelShader = pso.pixelShader;
        geometryShader = pso.geometryShader;
        hullShader = pso.hullShader;
        domainShader = pso.domainShader;
        rasterizerState = pso.rasterizerState;
        inputLayout = pso.inputLayout;
        samplerState = pso.samplerState;
        primitiveTopology = pso.primitiveTopology;
        depthStencilState = pso.depthStencilState;
    }

public:
    VertexShader vertexShader;
    PixelShader pixelShader;
    GeometryShader geometryShader;
    HullShader hullShader;
    DomainShader domainShader;
    RasterizerState rasterizerState;
    SamplerState samplerState;
    InputLayout inputLayout;
    DepthStencilState depthStencilState;
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};