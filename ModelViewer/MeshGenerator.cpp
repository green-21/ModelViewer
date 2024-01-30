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

MeshData MeshGenerator::GenerateAxisGrid(const int halfLength, const int unit) {
    MeshData grid;

    // XZ Grid
    Vector3 color(0.7f);
    for (int x = -halfLength; x <= halfLength; x += unit) {
        Vertex v1 = {{float(x), 0.0f, float(-halfLength)}, color, {0.0f, 0.0f}};
        Vertex v2 = {{float(x), 0.0f, float(halfLength)},
                     color,

                     {0.0f, 0.0f}};
        grid.vertices.push_back(v1);
        grid.vertices.push_back(v2);
    }

    for (int z = -halfLength; z <= halfLength; z += unit) {
        Vertex v1 = {{float(-halfLength), 0.0f, float(z)},
                     color,

                     {0.0f, 0.0f}};
        Vertex v2 = {{float(halfLength), 0.0f, float(z)},
                     color,

                     {0.0f, 0.0f}};
        grid.vertices.push_back(v1);
        grid.vertices.push_back(v2);
    }

    // axis
    grid.vertices.push_back(
        {{float(-halfLength), 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    grid.vertices.push_back(
        {{float(halfLength), 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});

    grid.vertices.push_back(
        {{0.0f, float(-halfLength), 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
    grid.vertices.push_back(
        {{0.0f, float(halfLength), 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});

    grid.vertices.push_back(
        {{0.0f, 0.0f, float(-halfLength)}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}});
    grid.vertices.push_back(
        {{0.0f, 0.0f, float(halfLength)}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}});

    const int lineCount = int(grid.vertices.size());
    grid.indices.resize(lineCount);

    for (int i = 0; i < lineCount; i++) {
        grid.indices[i] = i;
    }

    return grid;
}
