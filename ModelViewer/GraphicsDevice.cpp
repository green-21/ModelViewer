#include "GraphicsDevice.h"

std::unique_ptr<Renderer>
GraphicsDevice::InitAndCreateRenderer(HWND windowHandle, int width,
                                      int height) {
    this->width = width;
    this->height = height;

    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_9_3,
    };

    D3D_FEATURE_LEVEL featureLevel;
    DXGI_SWAP_CHAIN_DESC scDesc{};
    scDesc.BufferCount = 2;
    scDesc.BufferDesc.Width = width;
    scDesc.BufferDesc.Height = height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.OutputWindow = windowHandle;
    scDesc.Windowed = true;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;

    // Renderer
    Context context;
    SwapChain swapChain;
    RenderTargetView renderTargetView;
    DepthStencilView depthStencilView;
    TextureBuffer2D depthStencilBuffer;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags,
        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &scDesc,
        swapChain.GetAddressOf(), this->device.GetAddressOf(), &featureLevel,
        context.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "failed to create device" << std::endl;
        return nullptr;
    }

    TextureBuffer2D backBuffer;
    hr = swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        std::cerr << "failed to get backBuffer from swapChain." << std::endl;
        return nullptr;
    }

    renderTargetView = CreateRenderTargetView(backBuffer);

    // TODO: �ش� �Լ����� �и��� �� �ִ� ����� ���
    D3D11_TEXTURE2D_DESC dsDesc{};
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.ArraySize = 1;
    dsDesc.MipLevels = 1;
    dsDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    dsDesc.CPUAccessFlags = 0;
    dsDesc.MiscFlags = 0;
    dsDesc.Usage = D3D11_USAGE_DEFAULT;

    depthStencilBuffer = CreateTextureBuffer2D(dsDesc, nullptr);

    hr = this->device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr,
                                              depthStencilView.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "failed to create DepthStencilView." << std::endl;
        return nullptr;
    }

    return std::make_unique<Renderer>(context, swapChain, renderTargetView,
                                      depthStencilBuffer, depthStencilView);
}

DepthStencilView GraphicsDevice::CreateDepthStencilView(TextureBuffer2D buffer) {
    DepthStencilView view;
    ThrowIfFailed(device->CreateDepthStencilView(buffer.Get(), nullptr,
                                                 view.GetAddressOf()));
    return view;
}

TextureBuffer2D
GraphicsDevice::CreateTextureBuffer2D(D3D11_TEXTURE2D_DESC &desc,
                                      const void *data, UINT pitch) {
    TextureBuffer2D texture;

    D3D11_SUBRESOURCE_DATA subResourceData{};
    subResourceData.pSysMem = data;
    subResourceData.SysMemPitch = pitch;

    auto *p = data ? &subResourceData : nullptr;
    ThrowIfFailed(device->CreateTexture2D(&desc, p, texture.GetAddressOf()));
    return texture;
}


ShaderResourceView
GraphicsDevice::CreateShaderResourceView(TextureBuffer2D texture) {
    ShaderResourceView view;
    ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), nullptr, view.GetAddressOf()));

    return view;
}

RenderTargetView GraphicsDevice::CreateRenderTargetView(TextureBuffer2D buffer) {
    RenderTargetView rtv;
    ThrowIfFailed(device->CreateRenderTargetView(buffer.Get(), nullptr,
                                                 rtv.GetAddressOf()));
    return rtv;
}

VertexShader GraphicsDevice::CreateVertexShader(const D3DBlob shaderBlob) {
    VertexShader vs;
    ThrowIfFailed(device->CreateVertexShader(shaderBlob->GetBufferPointer(),
                                             shaderBlob->GetBufferSize(),
                                             nullptr, vs.GetAddressOf()));
    return vs;
}

InputLayout GraphicsDevice::CreateInputLayout(
    const D3DBlob shaderBlob,
    std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements) {
    InputLayout il;
    ThrowIfFailed(device->CreateInputLayout(
        inputElements.data(), inputElements.size(),
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        il.GetAddressOf()));

    return il;
}

PixelShader GraphicsDevice::CreatePixelShader(const D3DBlob shaderBlob) {
    PixelShader ps;
    ThrowIfFailed(device->CreatePixelShader(shaderBlob->GetBufferPointer(),
                                            shaderBlob->GetBufferSize(),
                                            nullptr, ps.GetAddressOf()));
    return ps;
}

SamplerState GraphicsDevice::CreateSamplerState(D3D11_SAMPLER_DESC &desc) {
    SamplerState state;
    ThrowIfFailed(device->CreateSamplerState(&desc, state.GetAddressOf()));

    return state;
}

RasterizerState
GraphicsDevice::CreateRasterizerState(D3D11_RASTERIZER_DESC &desc) {
    RasterizerState state;
    ThrowIfFailed(device->CreateRasterizerState(&desc, state.GetAddressOf()));

    return state;
}

GraphicsBuffer GraphicsDevice::CreateGraphicsBuffer(D3D11_BUFFER_DESC &desc,
                                                    const void *data) {
    GraphicsBuffer buffer;
    D3D11_SUBRESOURCE_DATA subResourceData{};
    subResourceData.pSysMem = data;
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    auto *p = data ? &subResourceData : nullptr;
    ThrowIfFailed(
        device->CreateBuffer(&desc, p, buffer.GetAddressOf()));
    
    return buffer;
}