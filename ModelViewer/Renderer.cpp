#include "Renderer.h"

void Renderer::SetClearColor(Vector3 color) {
    clearColor[0] = color.x;
    clearColor[1] = color.y;
    clearColor[2] = color.z;
    clearColor[3] = 0.0f;
}

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
    context->PSSetSamplers(0, 1, pso.samplerState.GetAddressOf());
    context->IASetInputLayout(pso.inputLayout.Get());
    context->RSSetState(pso.rasterizerState.Get());
    context->IASetPrimitiveTopology(pso.primitiveTopology);
    context->OMSetDepthStencilState(pso.depthStencilState.Get(), 0);
}

void Renderer::ClearScreen() {
    context->ClearRenderTargetView(renderTargetView.Get(), clearColor);
    context->ClearDepthStencilView(depthStencilView.Get(),
                                   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                   1.0f, 0);
}

void Renderer::DrawIndexed(Model &model) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->VSSetConstantBuffers(0, 1,
                                  model.transformationBuffer.GetAddressOf());
    for (auto &node : model.nodes) {
        context->IASetVertexBuffers(0, 1, node.mesh.vertices.GetAddressOf(),
                                    &stride, &offset);

        context->IASetIndexBuffer(node.mesh.indices.Get(), DXGI_FORMAT_R32_UINT,
                                  0);
        context->PSSetShaderResources(0, 1, node.texture.view.GetAddressOf());
        context->DrawIndexed(node.mesh.indexCount, 0, 0);
    }
}
