#include "GraphicsDevice.h"
#include "MeshGenerator.h"

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
    TextureBuffer2D rawRenderBuffer;
    RenderTargetView rawRenderTargetView;
    ShaderResourceView rawShaderResourceView;

    RenderTargetView backBufferRenderTargetView;

    TextureBuffer2D depthStencilBuffer;
    DepthStencilView depthStencilView;
    ShaderResourceView depthShaderResourceView;

    TextureBuffer2D depthScreen;
    RenderTargetView depthScreenRenderTargetView;

    TextureBuffer2D depthMapBuffer;
    DepthStencilView depthMapView;
    ShaderResourceView depthMapShaderResourceView;

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

    backBufferRenderTargetView = CreateRenderTargetView(backBuffer);

    { // raw Buffer, Views
        D3D11_TEXTURE2D_DESC desc{};
        backBuffer->GetDesc(&desc);
        desc.ArraySize = 1;
        desc.MipLevels = 1;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        desc.MiscFlags = 0;
        desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

        rawRenderBuffer = CreateTextureBuffer2D(desc, nullptr);
        rawRenderTargetView = CreateRenderTargetView(rawRenderBuffer);
        rawShaderResourceView = CreateShaderResourceView(rawRenderBuffer);
    }

    D3D11_TEXTURE2D_DESC desc{};
    backBuffer->GetDesc(&desc);
    desc.ArraySize = 1;
    desc.MipLevels = 1;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    desc.MiscFlags = 0;
    desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

    depthScreen = CreateTextureBuffer2D(desc, nullptr);
    depthScreenRenderTargetView = CreateRenderTargetView(depthScreen);

    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    depthStencilBuffer = CreateTextureBuffer2D(desc, nullptr);
    depthStencilView = CreateDepthStencilView(depthStencilBuffer);

    desc.Format = DXGI_FORMAT_R32_TYPELESS;
    desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    depthMapBuffer = CreateTextureBuffer2D(desc, nullptr);
    depthMapView = CreateDepthStencilView(dsvDesc, depthMapBuffer);
    depthMapShaderResourceView =
        CreateShaderResourceView(srvDesc, depthMapBuffer);

    // TODO: 분리할 수 있는 방법 찾아보기
    // GraphicsDevice와 Renderer
    auto renderer = std::make_unique<Renderer>(context, swapChain);
    renderer->Init(rawRenderBuffer, rawRenderTargetView, rawShaderResourceView,
                   backBufferRenderTargetView, depthStencilBuffer,
                   depthStencilView, depthScreen, depthScreenRenderTargetView,
                   depthMapBuffer, depthMapView, depthMapShaderResourceView);

    MeshData mesh = MeshGenerator::Square();
    ModelMesh result;

    { // vertices
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(Vertex) * mesh.vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(Vertex);
        result.vertices = CreateGraphicsBuffer(desc, mesh.vertices.data());
    }

    { // indices
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(uint32_t) * mesh.indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(uint32_t);
        result.indices = CreateGraphicsBuffer(desc, mesh.indices.data());

        result.indexCount = UINT(mesh.indices.size());
    }
    renderer->square = result;
    return renderer;
}

DepthStencilView
GraphicsDevice::CreateDepthStencilView(TextureBuffer2D buffer) {
    DepthStencilView view;
    ThrowIfFailed(device->CreateDepthStencilView(buffer.Get(), nullptr,
                                                 view.GetAddressOf()));
    return view;
}

DepthStencilView
GraphicsDevice::CreateDepthStencilView(D3D11_DEPTH_STENCIL_VIEW_DESC &desc,
                                       TextureBuffer2D buffer) {
    DepthStencilView view;
    ThrowIfFailed(device->CreateDepthStencilView(buffer.Get(), &desc,
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
    ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), nullptr,
                                                   view.GetAddressOf()));

    return view;
}

ShaderResourceView
GraphicsDevice::CreateShaderResourceView(D3D11_SHADER_RESOURCE_VIEW_DESC &desc,
                                         TextureBuffer2D texture) {
    ShaderResourceView view;
    ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), &desc,
                                                   view.GetAddressOf()));

    return view;
}

RenderTargetView
GraphicsDevice::CreateRenderTargetView(TextureBuffer2D buffer) {
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
        inputElements.data(), UINT(inputElements.size()),
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

GeometryShader GraphicsDevice::CreateGeometryShader(const D3DBlob shaderBlob) {
    GeometryShader gs;
    ThrowIfFailed(device->CreateGeometryShader(shaderBlob->GetBufferPointer(),
                                               shaderBlob->GetBufferSize(),
                                               nullptr, gs.GetAddressOf()));
    return gs;
}

HullShader GraphicsDevice::CreateHullShader(const D3DBlob shaderBlob) {

    HullShader hs;
    ThrowIfFailed(device->CreateHullShader(shaderBlob->GetBufferPointer(),
                                           shaderBlob->GetBufferSize(), nullptr,
                                           hs.GetAddressOf()));
    return hs;
}

DomainShader GraphicsDevice::CreateDomainShader(const D3DBlob shaderBlob) {
    DomainShader ds;
    ThrowIfFailed(device->CreateDomainShader(shaderBlob->GetBufferPointer(),
                                             shaderBlob->GetBufferSize(),
                                             nullptr, ds.GetAddressOf()));
    return ds;
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
    ThrowIfFailed(device->CreateBuffer(&desc, p, buffer.GetAddressOf()));

    return buffer;
}

DepthStencilState
GraphicsDevice::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC &desc) {
    DepthStencilState state;
    ThrowIfFailed(device->CreateDepthStencilState(&desc, state.GetAddressOf()));

    return state;
}
