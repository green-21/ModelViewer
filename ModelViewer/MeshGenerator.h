#pragma once

#include "Model.h"

class MeshGenerator {
public:
    static MeshData GenerateCube();
    static MeshData GenerateAxisGrid(const int length, const int unit, Vector3 gridColor,
                                         float axisBrightness);
    static MeshData Square();
    static MeshData GenerateXZSquare(const float length);
};