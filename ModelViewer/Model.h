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

// CPU 상 존재하는 모델 데이터
struct MeshData {
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

// GPU 상 존재하는 모델 데이터
// 모델은 인스턴싱을 위헤 레퍼런스로 관리할 것.

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