#pragma once

#include <unordered_set>

#include "assimp/scene.h"
#include "Model.h"

class FileModel : public Model {
  public:
    explicit FileModel(const std::string &path);
    ~FileModel() override;

  private:
    std::string directory{};
    std::unordered_set<std::string> textures_loaded;
    void LoadFromFile(const std::string &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    void LoadMaterialTextures(Mesh &mesh, const aiMaterial *material, aiTextureType type, const std::string &typeName);
};
