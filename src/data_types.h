#pragma once

#include "program.hpp"
#include "shader.hpp"
#include "assimp/scene.h"
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 position{};
    // normal
    glm::vec3 normal{};
    // texCoords
    glm::vec2 texCoords{};
    // tangent
    glm::vec3 tangent{};
    // bitangent
    glm::vec3 bitangent{};
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE]{
        0,
    };
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE]{
        0,
    };
};

struct Texture {
    unsigned int id{};
    std::string type{};
    std::string path{};
};

class Mesh {
  public:
    std::vector<Vertex> vertices{};
    std::vector<unsigned int> indices{};
    std::vector<Texture> textures{};
    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Program &shader);

  private:
    unsigned int vao, vbo, ebo;
    void setupMesh();
};

class Model {
  public:
    void Draw(Program &shader);
    virtual ~Model() = default;
    static unsigned int TextureFromFile(const std::string &filename, const std::string &directory);

    std::vector<Mesh> meshes;
};

class FileModel : public Model {
  public:
    explicit FileModel(const std::string &path);
    ~FileModel() override;

  private:
    std::string directory{};
    std::unordered_map<std::string, Texture &> textures_loaded;
    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial *material, aiTextureType type,
                                              const std::string &typeName);
};

class MemoryModel : public Model {
  public:
    MemoryModel();
    void addMesh(Mesh &&mesh);
    ~MemoryModel() override;
};