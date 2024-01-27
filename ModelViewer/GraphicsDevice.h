#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "d3d11wrapper.h"

#include "Renderer.h"

using Microsoft::WRL::ComPtr;

class GraphicsDevice {
public:
    std::unique_ptr<Renderer> InitAndCreateRenderer(HWND windowHandle,
                                                    int width, int height);

    DepthStencilView CreateDepthStencilView(Texture2D buffer);

    Texture2D CreateTexture2D(D3D11_TEXTURE2D_DESC &desc,
                              const D3D11_SUBRESOURCE_DATA *pInitialData);

    RenderTargetView CreateRenderTargetView(Texture2D buffer);

private:
    int width, height;
    Device device;
};