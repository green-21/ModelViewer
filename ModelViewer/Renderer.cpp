#include "Renderer.h"

void Renderer::SetViewport(int width, int height) {
    viewport.Width = float(width);
    viewport.Height = float(height);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);
}

void Renderer::SetPipelineState(const GraphicsPipelineStateObject &pso) {
    context->VSSetShader(pso.vertexShader.Get(), 0, 0);
    context->PSSetShader(pso.pixelShader.Get(), 0, 0);
    context->IASetInputLayout(pso.inputLayout.Get());
    context->RSSetState(pso.rasterizerState.Get());
    context->IASetPrimitiveTopology(pso.primitiveTopology);
}

void Renderer::ClearScreen() {
    float clearColor[4] = {0.5f, 0.5f, 1.0f, 1.0f};
    context->ClearRenderTargetView(renderTargetView.Get(), clearColor);
    context->ClearDepthStencilView(depthStencilView.Get(),
                                     D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                     1.0f, 0);
}

void Renderer::Draw(GraphicsBuffer buffer) { 
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(),
                                      &stride, &offset);

    context->Draw(3,0);
}
