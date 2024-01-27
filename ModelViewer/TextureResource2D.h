#pragma once

#include "d3d11wrapper.h"

struct TextureResource2D {
    TextureBuffer2D buffer;
    ShaderResourceView view;
};