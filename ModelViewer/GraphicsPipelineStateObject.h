#pragma once

#include<vector>
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
        blendState = pso.blendState;
        vertexShaderConstantBuffers = pso.vertexShaderConstantBuffers;
        pixelShaderConstantBuffers = pso.pixelShaderConstantBuffers;
        geometryShaderConstantBuffers = pso.geometryShaderConstantBuffers;
        hullShaderConstantBuffers = pso.hullShaderConstantBuffers;
        domainShaderConstantBuffers = pso.domainShaderConstantBuffers;
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
    BlendState blendState;

    std::vector<GraphicsBuffer> vertexShaderConstantBuffers;
    std::vector<GraphicsBuffer> pixelShaderConstantBuffers;
    std::vector<GraphicsBuffer> geometryShaderConstantBuffers;
    std::vector<GraphicsBuffer> hullShaderConstantBuffers;
    std::vector<GraphicsBuffer> domainShaderConstantBuffers;
};