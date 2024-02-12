#include "ResourceManager.h"
#include "FileIO.h"

ModelMesh ResourceManager::CreateMesh(const std::string &name,
                                 const MeshData &mesh) {
    meshStorage.Insert(name, createMesh(mesh));
    return meshStorage.Get(name);
}

Model ResourceManager::CreateModelFromStorage(
    const std::string &name, std::vector<std::string> &&meshNames,
    std::vector<std::string> &&textureNames) {
    return CreateModelFromStorage(name, meshNames, textureNames);
}

Model ResourceManager::CreateModelFromStorage(
    const std::string &name, std::vector<std::string> &meshNames,
    std::vector<std::string> &textureNames) {

    std::vector<ModelNode> nodes;

    for (auto meshName : meshNames) {
        nodes.push_back(ModelNode());
        nodes.back().mesh = meshStorage.Get(meshName);
    }

    const int size = std::min(int(textureNames.size()), int(nodes.size()));
    for (int i = 0; i < size; i++) {
        nodes[i].texture = textureStorage.Get(textureNames[i]);
    }

    modelStorage.Insert(name, Model{nodes});
    return modelStorage.Get(name);
}

void ResourceManager::LoadTextureFromFile(const std::string &name,
                                          const std::string &filepath) {

    Image image = FileIO::ReadImage(filepath);
    textureStorage.Insert(name, std::move(createTexture(image)));
}

void ResourceManager::LoadModelFromFile(const std::string &name,
                                        const std::string &filepath) {
    ModelLoader loader;
    std::vector<MeshData> raws = loader.LoadModel(filepath);

    Model model;
    for (auto &raw : raws) {
        ModelNode node;
        node.mesh = createMesh(raw);
        node.texture = createTexture(FileIO::ReadImage(raw.texturePath));

        model.nodes.push_back(node);
    }
    modelStorage.Insert(name, std::move(model));
}

TextureResource2D ResourceManager::GetTexture2D(const std::string &name) {
    return textureStorage.Get(name);
}

ModelMesh ResourceManager::GetMesh(const std::string &name) {
    return meshStorage.Get(name);
}

Model ResourceManager::GetModel(const std::string &name) {
    return modelStorage.Get(name);
}

ModelMesh ResourceManager::createMesh(const MeshData &mesh) {
    ModelMesh result;

    { // vertices
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(Vertex) * mesh.vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(Vertex);
        result.vertices = device->CreateGraphicsBuffer(desc, mesh.vertices.data());
    }

    { // indices
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = UINT(sizeof(uint32_t) * mesh.indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.StructureByteStride = sizeof(uint32_t);
        result.indices = device->CreateGraphicsBuffer(desc, mesh.indices.data());

        result.indexCount = UINT(mesh.indices.size());
    }

    return result;
}

TextureResource2D ResourceManager::createTexture(Image image) {
    TextureResource2D resource;

    D3D11_TEXTURE2D_DESC desc{};
    desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Width = image.GetWidth();
    desc.Height = image.GetHeight();
    desc.MipLevels = 1;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;

    resource.buffer = device->CreateTextureBuffer2D(
        desc, image.GetData(),
        (image.GetWidth() * sizeof(uint8_t) * image.GetChannel()));
    resource.view = device->CreateShaderResourceView(resource.buffer.Get());

    return resource;
}
