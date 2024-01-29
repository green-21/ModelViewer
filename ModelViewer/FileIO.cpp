#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "FileIO.h"
Image FileIO::ReadImage(const std::string filename) {
    int width, height, ch;
    constexpr int fullChannel = 4;
    unsigned char *raw = stbi_load(filename.c_str(), &width, &height, &ch, 0);
    size_t size = width * height;

    if (ch == 0) {
        std::exception(raw);
    }

    std::vector<uint8_t> img(size * fullChannel, 255);
    switch (ch) {
    case 1:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < fullChannel; c++) {
                img[i * fullChannel + c] = raw[i];
            }
        }
        break;
    case 2:
    case 3:
    case 4:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < ch; c++) {
                img[i * fullChannel + c] = raw[i * ch + c];
            }
        }
        break;
    }

    delete[] raw;
    return Image(width, height, fullChannel, img);
}

// reference: learnOpenGL
std::vector<MeshData> &&ModelLoader::LoadModel(const std::string filename) {
    Assimp::Importer importer;

    unsigned int flag = aiProcess_Triangulate |
                        aiProcess_JoinIdenticalVertices |
                        aiProcess_CalcTangentSpace | aiProcess_GenNormals |
                        aiProcess_FlipUVs;

    const aiScene *scene = importer.ReadFile(filename, flag);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::exception(importer.GetErrorString());
    }

    directory = filename.substr(0, filename.find_last_of('\\')) + '\\';
    processNode(scene->mRootNode, scene);

    return std::move(meshes);
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = processMesh(mesh, scene);

        meshes.push_back(newMesh);
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

MeshData ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
    MeshData result;

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
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            result.texturePath = this->directory + path.C_Str();
        }
    }

    return result;
}

//std::string ModelLoader::processMaterial(aiMesh *mesh, const aiScene *scene) {
//}
