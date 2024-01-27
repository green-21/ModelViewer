#pragma once

#include "d3d11wrapper.h"

class Renderer {
public:
    Renderer(Context context, SwapChain swapChain,
             RenderTargetView renderTargetView, Texture2D DepthStencilBuffer,
             DepthStencilView depthStencilView)
        : context(context), swapChain(swapChain),
          renderTargetView(renderTargetView),
          depthStencilBuffer(depthStencilBuffer),
          depthStencilView(depthStencilView) {}

private:
    Context context;
    SwapChain swapChain;

    RenderTargetView renderTargetView;
    Texture2D depthStencilBuffer;
    DepthStencilView depthStencilView;

    Viewport viewport{};
};