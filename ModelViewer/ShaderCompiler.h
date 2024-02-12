#pragma once

#include <iostream>
#include <string>

#include "d3d11wrapper.h"

class ShaderCompiler {
public:
    static D3DBlob Compile(std::wstring filename, std::string shaderType) {
        D3DBlob shaderBlob;
        D3DBlob errorBlob;
        UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
        compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        D3DCompileFromFile(filename.c_str(), nullptr,
                           D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
                           shaderType.c_str(), compileFlags, 0,
                           shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

        if (errorBlob) {
            std::cerr << "failed to compile shader : "
                      << static_cast<char *>(errorBlob->GetBufferPointer())
                      << std::endl;
        }

        return shaderBlob;
    }
};