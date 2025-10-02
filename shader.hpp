#pragma once

#include <fstream>
#include <glad/glad.h>
#include <regex>
#include <string>

#include "log.h"

class Shader {
  public:
    Shader(GLenum shaderType, const std::string &src);

    ~Shader();

    bool Init();

    bool Check() const;

    operator GLuint() const;

    operator bool() const;

    bool IsInitialized() const { return initialized; }

  private:
    bool GetShaderSource();

    std::string src;
    GLuint shader_id;
    GLenum shader_type;
    bool initialized = false;
};