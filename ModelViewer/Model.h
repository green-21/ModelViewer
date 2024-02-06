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
        : view(Matrix()), projection(Matrix()), invProjection(Matrix()) {}

    void Transpose() {
        view = view.Transpose();
        projection = projection.Transpose();
        invProjection = invProjection.Transpose();
    }

    Matrix view;
    Matrix projection;
    Matrix invProjection;
    Vector3 cameraPos;
    int padding;
};

struct ModelTransformationMatrix {
    ModelTransformationMatrix() : model(Matrix()) {}

    void Transpose() { model = model.Transpose(); }

    Matrix model;
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
    UINT indexCount = 0;
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
    GraphicsBuffer transformationBuffer;
};