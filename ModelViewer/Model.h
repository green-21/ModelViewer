#pragma once

#include <string>

#include "TextureResource2D.h"
#include "d3d11wrapper.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};

struct TransformationMatrix {
    TransformationMatrix() {
        model = Matrix();
        view = Matrix();
        projection = Matrix();
    }

    void Transpose() {
        model = model.Transpose();
        view = view.Transpose();
        projection = projection.Transpose();
    }

    Matrix model;
    Matrix view;
    Matrix projection;
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
    UINT indexCount;
    GraphicsBuffer vertices;
    GraphicsBuffer indices;
};

struct ModelNode {
    ModelMesh mesh;
    TextureResource2D texture;
};

struct Model {
    std::vector<ModelNode> nodes;
    TransformationMatrix transformationMatrix;
    GraphicsBuffer transformationBuffer;
};