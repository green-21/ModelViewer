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
struct Mesh {
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<TextureBuffer2D> textures;
};

// GPU 상 존재하는 모델 데이터
// 모델은 인스턴싱을 위헤 레퍼런스로 관리할 것.
struct Model {
    UINT indexCount;
    TextureResource2D texture;
    GraphicsBuffer vertices;
    GraphicsBuffer indices;
    TransformationMatrix transformationMatrix;
    GraphicsBuffer transformationBuffer;
};