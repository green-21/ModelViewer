#include "Renderer.h"


void Renderer::UpdateCameraMatrix(CameraTransformationMatrix matrix) {
    UpdateBuffer(cameraTransformBuffer, matrix);
}

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
    context->GSSetShader(pso.geometryShader.Get(), 0, 0);
    context->HSSetShader(pso.hullShader.Get(), 0, 0);
    context->DSSetShader(pso.domainShader.Get(), 0, 0);
    context->PSSetSamplers(0, 1, pso.samplerState.GetAddressOf());
    context->IASetInputLayout(pso.inputLayout.Get());
    context->RSSetState(pso.rasterizerState.Get());
    context->IASetPrimitiveTopology(pso.primitiveTopology);
    context->OMSetDepthStencilState(pso.depthStencilState.Get(), 0);
    context->OMSetBlendState(pso.blendState.Get(), nullptr, 0xffffffff);
}

void Renderer::ClearScreen() {
    context->ClearRenderTargetView(rawRenderTargetView.Get(), clearColor);
    context->ClearRenderTargetView(depthScreenRenderTargetView.Get(),
                                   clearColor);
    context->ClearDepthStencilView(depthMapView.Get(), D3D11_CLEAR_DEPTH, 1.0f,
                                   0);
    context->ClearDepthStencilView(depthStencilView.Get(),
                                   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                   1.0f, 0);
}

void Renderer::DrawIndexed(Model &model) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    std::vector<ID3D11Buffer *> buffers = {cameraTransformBuffer.Get(),
                                           model.transformationBuffer.Get()};
    context->VSSetConstantBuffers(0, UINT(buffers.size()), buffers.data());
    context->GSSetConstantBuffers(0, UINT(buffers.size()), buffers.data());
    context->HSSetConstantBuffers(0, UINT(buffers.size()), buffers.data());
    context->DSSetConstantBuffers(0, UINT(buffers.size()), buffers.data());

    context->OMSetRenderTargets(1, rawRenderTargetView.GetAddressOf(),
                                depthStencilView.Get());
    for (auto &node : model.nodes) {
        context->IASetVertexBuffers(0, 1, node.mesh.vertices.GetAddressOf(),
                                    &stride, &offset);

        context->IASetIndexBuffer(node.mesh.indices.Get(), DXGI_FORMAT_R32_UINT,
                                  0);
        context->PSSetShaderResources(0, 1, node.texture.view.GetAddressOf());
        context->DrawIndexed(node.mesh.indexCount, 0, 0);
    }

    context->OMSetRenderTargets(1, depthScreenRenderTargetView.GetAddressOf(),
                                depthMapView.Get());
    for (auto &node : model.nodes) {
        context->IASetVertexBuffers(0, 1, node.mesh.vertices.GetAddressOf(),
                                    &stride, &offset);

        context->IASetIndexBuffer(node.mesh.indices.Get(), DXGI_FORMAT_R32_UINT,
                                  0);
        context->PSSetShaderResources(0, 1, node.texture.view.GetAddressOf());
        context->DrawIndexed(node.mesh.indexCount, 0, 0);
    }
}

void Renderer::PostProcess() {

    context->OMSetRenderTargets(1, backBufferRenderTargetView.GetAddressOf(),
                                nullptr);

    SetPipelineState(postRenderPSO);
    std::vector<ID3D11ShaderResourceView *>views = {rawShaderResourceView.Get(),
                                         depthMapShaderResourceView.Get()};
    context->PSSetShaderResources(10, UINT(views.size()), views.data());
    context->PSSetConstantBuffers(0, 1, cameraTransformBuffer.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, square.vertices.GetAddressOf(), &stride,
                                &offset);
    context->IASetIndexBuffer(square.indices.Get(), DXGI_FORMAT_R32_UINT, 0);

    context->ClearRenderTargetView(backBufferRenderTargetView.Get(),
                                   clearColor);

    context->DrawIndexed(square.indexCount, 0, 0);
}
