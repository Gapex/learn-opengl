#include "../include/Model.h"
#include "../include/MemoryModel.h"
#include "../include/FileModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../include/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

FileModel::FileModel(const std::string &path) : Model() {
    LoadFromFile(path);
}

FileModel::~FileModel() = default;

void FileModel::ProcessNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::SetModelUniformName(const std::string &name) {
    this->modelMatrixName = name;
}

void Model::SetModelMatrix(const glm::mat4 &newModelMat) {
    this->modelMatrix = newModelMat;
}

const glm::mat4 &Model::GetModelMatrix() const {
    return modelMatrix;
}

unsigned int Model::TextureFromFile(const std::string &filename, const std::string &directory, GLint repeatType) {
    const std::string path = directory + '/' + filename;

    unsigned int textureID{};
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        switch (nrComponents) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            LOGE("Invalid nChannels: %d", nrComponents);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        LOGE("Texture failed to load at path: %s", path.c_str());
        stbi_image_free(data);
    }
    return textureID;
}

void FileModel::LoadMaterialTextures(Mesh &mesh, const aiMaterial *material, aiTextureType type,
                                     const std::string &typeName) {
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        const std::string filename = std::string(str.C_Str());
        if (textures_loaded.find(filename) != textures_loaded.end()) {
            continue;
        }
        Texture texture;
        texture.id = TextureFromFile(filename, directory);
        texture.uniformName = typeName + std::to_string(i);
        texture.type = GL_TEXTURE_2D;
        texture.path = filename;
        mesh.AddTexture(texture);
        textures_loaded.insert(filename);
    }
}

Mesh FileModel::ProcessMesh(const aiMesh *mesh, const aiScene *scene) {
    std::vector<float> vertices{}, normals{}, uvs{};
    std::vector<unsigned int> indices{};
    std::vector<Texture> textures{};
    vertices.reserve(mesh->mNumVertices * 3);
    normals.reserve(mesh->mNumVertices * 3);
    uvs.reserve(mesh->mNumVertices * 2);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // normals
        if (mesh->HasNormals()) {
            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            uvs.push_back(mesh->mTextureCoords[0][i].x);
            uvs.push_back(mesh->mTextureCoords[0][i].y);
        }
    }
    indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    Mesh result;
    result.SetIndices(indices);
    result.AddBuffer(0, BufferType::VERTEX_3D, vertices);
    if (!normals.empty()) {
        result.AddBuffer(BufferType::NORMAL, normals);
    }
    if (!uvs.empty()) {
        result.AddBuffer(BufferType::TEX_COORD, uvs);
    }
    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    LoadMaterialTextures(result, material, aiTextureType_DIFFUSE, "texture_diffuse");
    LoadMaterialTextures(result, material, aiTextureType_SPECULAR, "texture_specular");
    LoadMaterialTextures(result, material, aiTextureType_HEIGHT, "texture_normal");
    LoadMaterialTextures(result, material, aiTextureType_AMBIENT, "texture_height");
    return result;
}

void FileModel::LoadFromFile(const std::string &path) {
    Assimp::Importer import{};
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGE("Asssimp error: %s", import.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(Program &shader) {
    shader.SetMat4(modelMatrixName.c_str(), modelMatrix);
    for (Mesh &mesh : meshes) {
        if (!mesh.IsSetup()) {
            mesh.Setup();
        }
        mesh.Draw(shader);
    }
}

void MemoryModel::addMesh(Mesh &&mesh) {
    this->meshes.emplace_back(std::forward<Mesh>(mesh));
}
