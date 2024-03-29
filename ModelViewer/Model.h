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
          cameraPos(0.0f), padding(0) {}

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
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    GraphicsBuffer vertices;
    GraphicsBuffer indices;
};

struct ModelNode {
    ModelMesh mesh;
    TextureResource2D texture;
};

// ResourceManger에서 관리되는 모델 데이터
struct Model {
    std::vector<ModelNode> nodes;
    ModelTransformationMatrix transformationMatrix;
};

// 실제 그려질 모델 instance
struct ModelObject {
    ModelObject() = default;
    ModelObject(const char * name, Model model) : name(name), model(model) {}
    Matrix transformMatrix() const {
        return (Matrix::CreateTranslation(pos) * Matrix::CreateScale(scale)).Transpose();
    }
    std::string name;
    Model model;
    Vector3 pos{0.0f};
    Vector3 scale{1.0f};
};