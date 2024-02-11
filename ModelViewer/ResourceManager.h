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

    ModelMesh CreateMesh(const std::string &name, const MeshData &mesh);
    Model CreateModelFromStorage(const std::string &name,
                                 std::vector<std::string> &&meshNames,
                                 std::vector<std::string> &&textureNames);
    Model CreateModelFromStorage(const std::string &name,
                                 std::vector<std::string> &meshNames,
                                 std::vector<std::string> &textureNames);

    void LoadTextureFromFile(const std::string &name,
                             const std::string &filepath);
    void LoadModelFromFile(const std::string &name,
                           const std::string &filepath);

    TextureResource2D GetTexture2D(const std::string &name);
    ModelMesh GetMesh(const std::string &name);
    Model UseModel(const std::string &name);

private:
    ModelMesh createMesh(const MeshData &mesh);
    TextureResource2D createTexture(Image image);

private:
    std::shared_ptr<GraphicsDevice> device;

    ResourceStorage<TextureResource2D> textureStorage;
    ResourceStorage<ModelMesh> meshStorage;
    ResourceStorage<Model> modelStorage;
};