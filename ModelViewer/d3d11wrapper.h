#pragma once

#include <exception>

#include <Windows.h>
#include <wrl.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#include <directxtk/SimpleMath.h>

// 향후 wrapper나 다른 구현으로 변화해야한다면 여기에 추가하자.
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using Microsoft::WRL::ComPtr;

// device
using Device = ComPtr<ID3D11Device>;
using Context = ComPtr<ID3D11DeviceContext>;
using SwapChain = ComPtr<IDXGISwapChain>;

// view
using DepthStencilView = ComPtr<ID3D11DepthStencilView>;
using RenderTargetView = ComPtr<ID3D11RenderTargetView>;
using ShaderResourceView = ComPtr<ID3D11ShaderResourceView>;
using Viewport = D3D11_VIEWPORT;

// shader
using PixelShader = ComPtr<ID3D11PixelShader>;
using VertexShader = ComPtr<ID3D11VertexShader>;

using InputLayout = ComPtr<ID3D11InputLayout>;

// Buffer & Texture
using GraphicsBuffer = ComPtr<ID3D11Buffer>;
using TextureBuffer1D = ComPtr<ID3D11Texture1D>;
using TextureBuffer2D = ComPtr<ID3D11Texture2D>;
using TextureBuffer3D = ComPtr<ID3D11Texture3D>;

// state
using BlendState = ComPtr<ID3D11BlendState>;
using DepthStencilState = ComPtr<ID3D11DepthStencilState>;
using RasterizerState = ComPtr<ID3D11RasterizerState>;
using SamplerState = ComPtr<ID3D11SamplerState>;

// etc
using D3DBlob = ComPtr<ID3DBlob>;

inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        throw std::exception();
    }
}