#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "FileIO.h"
Image FileIO::ReadImage(const std::string filename) {
    int width, height, ch;
    unsigned char *raw = stbi_load(filename.c_str(), &width, &height, &ch, 0);
    size_t size = width * height;

    if (ch == 0) {
        std::exception(raw);
    }

    std::vector<uint8_t> img(size * 4, 255);
    switch (ch) {
    case 1:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < 4; c++) {
                img[i * 4 + c] = raw[i];
            }
        }
        break;
    case 2:
    case 3:
    case 4:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < ch; c++) {
                img[i * ch + c] = raw[i * ch + c];
            }
        }
        break;
    }

    delete[] raw;
    return Image(width, height, ch, img);
}

// reference: learnOpenGL
std::vector<Mesh> &&ModelLoader::LoadModel(const std::string filename) {
    Assimp::Importer importer;

    unsigned int flag =
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_FlipUVs;

    const aiScene *scene = importer.ReadFile(filename, flag);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::exception(importer.GetErrorString());
    }

    directory = filename.substr(0, filename.find_last_of('\\'));
    processNode(scene->mRootNode, scene);

    return std::move(meshes);
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
    Mesh result;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            v.normal.x = mesh->mNormals[i].x;
            v.normal.y = mesh->mNormals[i].y;
            v.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            v.uv.x = mesh->mTextureCoords[0][i].x;
            v.uv.y = mesh->mTextureCoords[0][i].y;
        }

        result.vertices.push_back(v);
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            result.indices.push_back(face.mIndices[j]);
        }
    }

    // materials
    if (mesh->mMaterialIndex >= 0) {
        // TODO //
    }

    return result;
}
