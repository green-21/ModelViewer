#include "MeshGenerator.h"

MeshData MeshGenerator::GenerateCube() {
    MeshData cube;
    cube.vertices = {
        // front
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // back
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // top
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

        // bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

        // right
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // left
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    cube.indices = {0,  1,  3,  1,  2,  3,  4,  7,  6,  4,  6,  5,
                    8,  11, 9,  11, 10, 9,  12, 14, 15, 12, 13, 14,
                    16, 19, 18, 16, 17, 19, 20, 22, 23, 20, 23, 21};

    return cube;
}

MeshData MeshGenerator::GenerateAxisGrid(const int halfLength, const int unit,
                                         Vector3 gridColor,
                                         float axisBrightness) {
    MeshData grid;

    // 0 1  4 5
    // 3 2  7 6
    // 위와 같은 형식으로 사각형 순서로 구성
    // 1과 4는 같은 위치에 존재.
    const float delta = halfLength / 5;
    const float dx[] = {0, delta, delta, 0};
    const float dz[] = {0, 0, delta, delta};
    for (float x = -halfLength; x < halfLength; x += delta) {
        for (float z = -halfLength; z < halfLength; z += delta) {
            for (int i = 0; i < 4; i++) {
                Vertex v = {
                    {x + dx[i], 0.0f, z + dz[i]}, gridColor, {0.0f, 0.0f}};
                grid.vertices.push_back(v);
            }
        }
    }

    for (uint32_t i = 0; i < grid.vertices.size(); i += 4) {
        grid.indices.insert(grid.indices.end(),
                            {i, i + 1, i + 1, i + 2, i + 2, i + 3, i + 3, i});
    }

    return grid;
}

MeshData MeshGenerator::GenerateXZSquare(const float length) {
    MeshData square;

    square.vertices = {
        {{-length, 0.0f, length}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{length, 0.0f, length}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{length, 0.0f, -length}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-length, 0.0f, -length}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };
    square.indices = {0, 1, 3, 1, 2, 3};

    return square;
}

MeshData MeshGenerator::Square() {
    MeshData square;
    square.vertices = {
        {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };

    square.indices = {0, 1, 3, 1, 2, 3};

    return square;
}
