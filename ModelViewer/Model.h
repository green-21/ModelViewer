#pragma once

#include <string>

#include "d3d11wrapper.h"
#include "TextureResource2D.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};


// CPU 상 존재하는 모델 데이터
struct Mesh {
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};


// GPU 상 존재하는 모델 데이터
// 모델은 인스턴싱을 위헤 레퍼런스로 관리할 것.
struct Model {
    UINT indexCount;
    TextureResource2D texture;
    GraphicsBuffer vertices;
    GraphicsBuffer indices;
};