#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Image.h"
#include "Model.h"

class FileIO {
public:
    static Image ReadImage(const std::string filename);
};


// reference: LearnOpenGL
class ModelLoader {
public:
    std::vector<MeshData> &&LoadModel(const std::string filename);

private:
    void processNode(aiNode *node, const aiScene *scene);
    MeshData processMesh(aiMesh *mesh, const aiScene *scene);
    //std::string processMaterial(aiMesh *mesh, const aiScene *scene);

    std::vector<MeshData> meshes;
    std::string directory;
};