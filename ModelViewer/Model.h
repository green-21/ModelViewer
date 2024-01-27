#pragma once

#include <string>

#include "d3d11wrapper.h"
#include "TextureResource2D.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};


// CPU �� �����ϴ� �� ������
struct Mesh {
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};


// GPU �� �����ϴ� �� ������
// ���� �ν��Ͻ��� ���� ���۷����� ������ ��.
struct Model {
    UINT indexCount;
    TextureResource2D texture;
    GraphicsBuffer vertices;
    GraphicsBuffer indices;
};