#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

class Program {
  public:
    Program() = default;

    bool Init();

    [[nodiscard]] bool Check() const;

    void Use() const;

    void AddShader(const std::shared_ptr<Shader> &shader);

    void AddShader(GLint shaderType, const std::string &path);

    [[nodiscard]] GLuint GetId() const { return program_id; }

    void SetInt(const char *name, int value) const;

    void SetMat4(const char *name, const glm::mat4 &mat) const;

    void SetFloat(const char *name, float value) const;

    void SetVec3(const char *name, glm::vec3 value) const;

    template<typename T>
    void SetUniform(const char *name, const T &value) const;

    template<>
    void SetUniform(const char *name, const float &value) const {
        glUniform1f(GetUniformLocation(name), value);
    }

    template<>
    void SetUniform(const char *name, const int &value) const {
        glUniform1i(GetUniformLocation(name), value);
    }

    template<>
    void SetUniform(const char *name, const unsigned int &value) const {
        glUniform1ui(GetUniformLocation(name), value);
    }

    template<>
    void SetUniform(const char *name, const bool &value) const {
        glUniform1i(GetUniformLocation(name), value);
    }

    template<>
    void SetUniform(const char *name, const glm::vec3 &value) const {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }
    template<>
    void SetUniform(const char *name, const glm::vec4 &value) const {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }
    template<>
    void SetUniform(const char *name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<>
    void SetUniform(const char *name, const glm::vec2 &value) const {
        glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }

  private:

    GLint GetUniformLocation(const char *name) const;

    std::vector<std::shared_ptr<Shader>> shaders{};
    GLuint program_id{};
    bool initialized{false};
};
