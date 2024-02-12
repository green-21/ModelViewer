#include <algorithm>

#include "Renderer.h"

void Renderer::SetCameraMatrix(GraphicsBuffer buffer, UINT slot) {
    cameraTransformBuffer = buffer;
    context->VSSetConstantBuffers(slot, 1,
                                  cameraTransformBuffer.GetAddressOf());
    context->PSSetConstantBuffers(slot, 1,
                                  cameraTransformBuffer.GetAddressOf());
}

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
    this->pso = &pso;
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

    // TODO: 코드 개선 할 방법 찾아보기....
    std::vector<ID3D11Buffer *> vs(pso.vertexShaderConstantBuffers.size());
    std::transform(pso.vertexShaderConstantBuffers.begin(),
                   pso.vertexShaderConstantBuffers.end(), vs.begin(),
                   [](const GraphicsBuffer &ptr) { return ptr.Get(); });
    context->VSSetConstantBuffers(1, UINT(vs.size()), vs.data());

    std::vector<ID3D11Buffer *> ps(pso.pixelShaderConstantBuffers.size());
    std::transform(pso.pixelShaderConstantBuffers.begin(),
                   pso.pixelShaderConstantBuffers.end(), ps.begin(),
                   [](const GraphicsBuffer &ptr) { return ptr.Get(); });
    context->VSSetConstantBuffers(1, UINT(ps.size()), ps.data());

    std::vector<ID3D11Buffer *> gs(pso.geometryShaderConstantBuffers.size());
    std::transform(pso.geometryShaderConstantBuffers.begin(),
                   pso.geometryShaderConstantBuffers.end(), gs.begin(),
                   [](const GraphicsBuffer &ptr) { return ptr.Get(); });
    context->VSSetConstantBuffers(1, UINT(gs.size()), gs.data());

    std::vector<ID3D11Buffer *> hs(pso.hullShaderConstantBuffers.size());
    std::transform(pso.hullShaderConstantBuffers.begin(),
                   pso.hullShaderConstantBuffers.end(), hs.begin(),
                   [](const GraphicsBuffer &ptr) { return ptr.Get(); });
    context->VSSetConstantBuffers(1, UINT(hs.size()), hs.data());

    std::vector<ID3D11Buffer *> ds(pso.domainShaderConstantBuffers.size());
    std::transform(pso.domainShaderConstantBuffers.begin(),
                   pso.domainShaderConstantBuffers.end(), ds.begin(),
                   [](const GraphicsBuffer &ptr) { return ptr.Get(); });
    context->VSSetConstantBuffers(1, UINT(ds.size()), ds.data());
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

void Renderer::DrawIndexed(ModelObject &obj) {
    
    UpdateBuffer(pso->vertexShaderConstantBuffers[0], obj.transformMatrix());
    context->OMSetRenderTargets(1, rawRenderTargetView.GetAddressOf(),
                                depthStencilView.Get());
    for (auto &node : obj.model.nodes) {
        drawIndexed(node);
    }

    context->OMSetRenderTargets(1, depthScreenRenderTargetView.GetAddressOf(),
                                depthMapView.Get());
    for (auto &node : obj.model.nodes) {
        drawIndexed(node);
    }
}

void Renderer::PostProcess() {

    context->OMSetRenderTargets(1, backBufferRenderTargetView.GetAddressOf(),
                                nullptr);

    SetPipelineState(postRenderPSO);
    std::vector<ID3D11ShaderResourceView *> views = {
        rawShaderResourceView.Get(), depthMapShaderResourceView.Get()};
    context->PSSetShaderResources(10, UINT(views.size()), views.data());
    context->PSSetConstantBuffers(0, 1, cameraTransformBuffer.GetAddressOf());

    drawIndexed(square);
}

void Renderer::drawIndexed(ModelNode &node) {
    context->IASetVertexBuffers(0, 1, node.mesh.vertices.GetAddressOf(),
                                &node.mesh.stride, &node.mesh.offset);

    context->IASetIndexBuffer(node.mesh.indices.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->PSSetShaderResources(0, 1, node.texture.view.GetAddressOf());
    context->DrawIndexed(node.mesh.indexCount, 0, 0);
}
