#pragma once

#include "Model.h"

class MeshGenerator {
public:
    static MeshData GenerateCube();
    static MeshData GenerateAxisGrid(const int length, const int unit);
};