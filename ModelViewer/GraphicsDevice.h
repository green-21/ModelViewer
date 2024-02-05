#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Image.h"
#include "Model.h"
#include "Renderer.h"
#include "TextureResource2D.h"
#include "d3d11wrapper.h"

using Microsoft::WRL::ComPtr;

class GraphicsDevice {
public:
    std::unique_ptr<Renderer> InitAndCreateRenderer(HWND windowHandle,
                                                    int width, int height);

    DepthStencilView CreateDepthStencilView(TextureBuffer2D buffer);
    DepthStencilView
    CreateDepthStencilView(D3D11_DEPTH_STENCIL_VIEW_DESC &desc, TextureBuffer2D buffer);
    TextureBuffer2D CreateTextureBuffer2D(D3D11_TEXTURE2D_DESC &desc,
                                          const void *data, UINT pitch=0);

    ShaderResourceView CreateShaderResourceView(TextureBuffer2D texture);
    ShaderResourceView CreateShaderResourceView(D3D11_SHADER_RESOURCE_VIEW_DESC&desc, TextureBuffer2D texture);

    RenderTargetView CreateRenderTargetView(TextureBuffer2D buffer);

    VertexShader CreateVertexShader(const D3DBlob shaderBlob);
    InputLayout
    CreateInputLayout(const D3DBlob shaderBlob,
                      std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements);
    PixelShader CreatePixelShader(const D3DBlob shaderBlob);

    SamplerState CreateSamplerState(D3D11_SAMPLER_DESC &desc);

    RasterizerState CreateRasterizerState(D3D11_RASTERIZER_DESC &desc);

    template <typename T> GraphicsBuffer CreateConstantBuffer(const T& data) {
        D3D11_BUFFER_DESC desc;
        desc.ByteWidth = sizeof(data);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        return CreateGraphicsBuffer(desc, &data);
    }
    GraphicsBuffer CreateGraphicsBuffer(D3D11_BUFFER_DESC &desc,
                                        const void *data);

    DepthStencilState CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC &desc);

    auto Get() { return device.Get(); }

private:
    int width, height;
    Device device;
};