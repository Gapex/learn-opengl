#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "shader.hpp"
#include "glm/gtc/type_ptr.hpp"

class Program {
  public:
    Program() = default;

    bool Init();

    bool Check() const;

    void Activate() const ;

    void Append(const std::shared_ptr<Shader> &shader);

    GLuint GetId() const {
      return program_id;
  }

    void SetInt(const char *name, int value) const;

    void SetMat4(const char *name, const glm::mat4 &mat) const;

    void SetFloat(const char *name, float value) const;

  private:
    std::vector<std::shared_ptr<Shader>> shaders{};
    GLuint program_id{};
    bool initialized{false};
};
