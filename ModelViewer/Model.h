#pragma once

#include <memory>
#include <string>

#include "TextureResource2D.h"
#include "d3d11wrapper.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};

struct CameraTransformationMatrix {
    CameraTransformationMatrix()
        : view(Matrix()), projection(Matrix()), invProjection(Matrix()),
          cameraPos(0.0f), padding(0)
    {}

    void Transpose() {
        view = view.Transpose();
        projection = projection.Transpose();
        invProjection = invProjection.Transpose();
    }

    Matrix view;
    Matrix projection;
    Matrix invProjection;
    Vector3 cameraPos{0.0f};
    int padding = 0;
};

struct ModelTransformationMatrix {
    ModelTransformationMatrix() : model(Matrix()) {}

    void Transpose() { model = model.Transpose(); }

    Matrix model;
};

// CPU �� �����ϴ� �� ������
struct MeshData {
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

// GPU �� �����ϴ� �� ������
// ���� �ν��Ͻ��� ���� ���۷����� ������ ��.
struct ModelMesh {
    UINT indexCount = 0;
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    GraphicsBuffer vertices;
    GraphicsBuffer indices;
};

struct ModelNode {
    ModelMesh mesh;
    TextureResource2D texture;
};

struct Model {
    std::vector<ModelNode> nodes;
    ModelTransformationMatrix transformationMatrix;
}; 