#pragma once

#include "GraphicsPipelineStateObject.h"
#include "d3d11wrapper.h"

class Renderer {
public:
    Renderer(Context context, SwapChain swapChain,
             RenderTargetView renderTargetView, TextureBuffer2D DepthStencilBuffer,
             DepthStencilView depthStencilView)
        : context(context), swapChain(swapChain),
          renderTargetView(renderTargetView),
          depthStencilBuffer(depthStencilBuffer),
          depthStencilView(depthStencilView) {

        context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(),
                                      depthStencilView.Get());
    }

    void SetViewport(int width, int height);
    void SetPipelineState(const GraphicsPipelineStateObject &pso);
    void ClearScreen();
    void Draw(GraphicsBuffer buffer);
    void Present() { swapChain->Present(1, 0); }

    auto GetContext() { return context.Get(); }

private:
    Context context;
    SwapChain swapChain;

    RenderTargetView renderTargetView;
    TextureBuffer2D depthStencilBuffer;
    DepthStencilView depthStencilView;

    Viewport viewport{};

    GraphicsPipelineStateObject pso;
};