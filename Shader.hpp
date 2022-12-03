#pragma once
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <regex>
#include <string>

#include "Log.hpp"

class Shader {
public:
  Shader(GLenum shaderType, const std::string &src)
      : src(src), shaderType(shaderType) {}

  ~Shader() { glDeleteShader(shaderId); }

  bool Init() {
    if (initialized) {
      log_d("%s:%s: %s: %u", __FILE_NAME__, __LINE__, "initialized shader",
            shaderId);
      return false;
    }
    initialized = true;
    shaderId = glCreateShader(shaderType);
    if (!GetShaderSource()) {
      return false;
    }
    const char *srcData = src.data();
    std::cout << src << std::endl;
    glShaderSource(shaderId, 1, &srcData, nullptr);
    glCompileShader(shaderId);
    return Check();
  }

  bool Check() const {
    bool result = true;
    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
      char infoLog[1024]{
          0,
      };
      glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
      std::cerr << src << std::endl << infoLog << std::endl;
      result = false;
    }
    return result;
  }

  operator GLuint() const { return shaderId; }

  operator bool() const { return IsInitialized(); }

  bool IsInitialized() const { return initialized; }

private:
  bool GetShaderSource() {
    static std::regex shaderSourceRegex(R"(#version.*void.*main)");
    std::smatch match;
    if (std::regex_search(src, match, shaderSourceRegex)) {
      return true;
    }
    std::ifstream file(src);
    if (!file.is_open()) {
      return false;
    }
    src.clear();
    std::string buffer;
    while (getline(file, buffer)) {
      if (!buffer.empty()) {
        src += buffer;
      }
      src.push_back('\n');
    }
    return true;
  }

  std::string src;
  GLuint shaderId;
  GLenum shaderType;
  bool initialized = false;
};