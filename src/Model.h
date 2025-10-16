#pragma once

#include "Program.h"
#include "Mesh.h"

class Model {
  public:
    void Draw(Program &shader);
    virtual ~Model() = default;
    static unsigned int TextureFromFile(const std::string &filename, const std::string &directory,
                                        GLint repeatType = GL_REPEAT);
    void SetModelUniformName(const std::string &name);
    void SetModelMatrix(const glm::mat4 &newModelMat);
    [[nodiscard]] const glm::mat4 &GetModelMatrix() const;
    std::vector<Mesh> meshes;

  protected:
    std::string modelMatrixName = "model";
    glm::mat4 modelMatrix{1.0f};
};