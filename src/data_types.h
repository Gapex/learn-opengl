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
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw(Program &shader);
    void Setup();
    [[nodiscard]] bool IsSetup() const { return isSetup; }

    ~Mesh();

  private:
    bool isSetup{false};
    unsigned int vao{}, vbo{}, ebo{};
};

class Model {
  public:
    void Draw(Program &shader);
    virtual ~Model() = default;
    static unsigned int TextureFromFile(const std::string &filename, const std::string &directory, GLint repeatType = GL_REPEAT);

    std::vector<Mesh> meshes;
};

class FileModel : public Model {
  public:
    explicit FileModel(const std::string &path);
    ~FileModel() override;

  private:
    std::string directory{};
    std::unordered_map<std::string, Texture &> textures_loaded;
    void LoadFromFile(const std::string &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(const aiMaterial *material, aiTextureType type,
                                              const std::string &typeName);
};

class MemoryModel : public Model {
  public:
    MemoryModel();
    void addMesh(Mesh &&mesh);
    ~MemoryModel() override;
};