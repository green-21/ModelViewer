#pragma once

#include "GraphicsPipelineStateObject.h"
#include "Model.h"
#include "d3d11wrapper.h"

class Renderer {
public:
    Renderer(Context context, SwapChain swapChain)
        : context(context), swapChain(swapChain) {}
    void Init(TextureBuffer2D rawRenderBuffer,
              RenderTargetView rawRenderTargetView,
              ShaderResourceView rawShaderResourceView,
              RenderTargetView backBufferRenderTargetView,
              TextureBuffer2D depthStencilBuffer,
              DepthStencilView depthStencilView, TextureBuffer2D depthScreen,
              RenderTargetView depthScreenRenderTargetView,
              TextureBuffer2D depthMapBuffer, DepthStencilView depthMapView,
              ShaderResourceView depthMapShaderResourceView) {

        this->rawRenderBuffer = rawRenderBuffer;
        this->rawRenderTargetView = rawRenderTargetView;
        this->rawShaderResourceView = rawShaderResourceView;
        this->backBufferRenderTargetView = backBufferRenderTargetView;
        this->depthStencilBuffer = depthStencilBuffer;
        this->depthStencilView = depthStencilView;
        this->depthScreen = depthScreen;
        this->depthScreenRenderTargetView = depthScreenRenderTargetView;
        this->depthMapBuffer = depthMapBuffer;
        this->depthMapView = depthMapView;
        this->depthMapShaderResourceView = depthMapShaderResourceView;

        context->OMSetRenderTargets(1, rawRenderTargetView.GetAddressOf(),
                                    depthStencilView.Get());
    }

    void SetCameraMatrix(GraphicsBuffer buffer, UINT slot = 0);
    void UpdateCameraMatrix(CameraTransformationMatrix matrix);
    void SetClearColor(Vector3 color);
    void SetViewport(int width, int height);
    void SetPipelineState(const GraphicsPipelineStateObject &pso);
    void ClearScreen();
    void DrawIndexed(ModelObject &obj);
    void SetPostRenderPSO(const GraphicsPipelineStateObject &pso) {
        postRenderPSO = pso;
    }
    void PostProcess();
    void Present() { swapChain->Present(1, 0); }
    template <typename T> void UpdateBuffer(GraphicsBuffer buffer, T &&data) {
        UpdateBuffer(buffer, data);
    }

    template <typename T> void UpdateBuffer(GraphicsBuffer buffer, T &data) {
        D3D11_MAPPED_SUBRESOURCE ms;

        ThrowIfFailed(context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD,
                                   NULL, &ms));
        memcpy(ms.pData, &data, sizeof(data));
        context->Unmap(buffer.Get(), NULL);
    }

    auto GetContext() { return context.Get(); }

public:
    ModelNode square;

private:
    void drawIndexed(ModelNode &node);

private:
    Context context;
    SwapChain swapChain;

    // TODO : RenderPathState에 대한 일반화 필요

    TextureBuffer2D rawRenderBuffer;
    RenderTargetView rawRenderTargetView;
    ShaderResourceView rawShaderResourceView;

    RenderTargetView backBufferRenderTargetView;

    TextureBuffer2D depthStencilBuffer;
    DepthStencilView depthStencilView;

    TextureBuffer2D depthScreen;
    RenderTargetView depthScreenRenderTargetView;

    TextureBuffer2D depthMapBuffer;
    DepthStencilView depthMapView;
    ShaderResourceView depthMapShaderResourceView;

    Viewport viewport{};
    float clearColor[4];

    GraphicsPipelineStateObject const *pso;
    GraphicsPipelineStateObject postRenderPSO;

    GraphicsBuffer cameraTransformBuffer;
};
