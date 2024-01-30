#pragma once

#include <memory>

#include "GraphicsDevice.h"
#include "Model.h"
#include "ResourceStorage.h"
#include "d3d11wrapper.h"

class ResourceManager {
public:
    ResourceManager(const std::shared_ptr<GraphicsDevice> device)
        : device(device) {}

    void CreateModelMeshAndTexture(const std::string &name, MeshData &mesh);
    Model CreateModelFromStroageData(const std::string &name);
    void LoadTextureFromFile(const std::string &name,
                             const std::string &filepath);
    void LoadModelFromFile(const std::string &name,
                           const std::string &filepath);

    TextureResource2D UseTexture2D(const std::string &name);
    ModelMesh UseModelMesh(const std::string &name);
    Model UseModel(const std::string &name);

private:
    ModelMesh createModelMesh(MeshData &mesh);
    TextureResource2D createTexture(Image image);

private:
    std::shared_ptr<GraphicsDevice> device;

    ResourceStorage<TextureResource2D> textureStorage;
    ResourceStorage<ModelMesh> meshStorage;
    ResourceStorage<Model> modelStorage;
};