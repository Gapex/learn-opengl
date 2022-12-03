#pragma once

#include <_types/_uint8_t.h>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

#include "Log.hpp"
#include "glad/glad.h"
#include "stb_image.h"

class Texture2D {
public:
  Texture2D(GLuint programId, GLuint vao, const std::string &filename)
      : filename(filename), programId(programId), vao(vao) {
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  bool Init() {
    unsigned char *dataPtr =
        stbi_load(filename.c_str(), &imgWidth, &imgHeight, &imgChannel, 0);
    if (dataPtr == nullptr) {
      std::cerr << "load texture failed: " << filename << std::endl;
      return false;
    }
    imgData = std::vector<uint8_t>(dataPtr,
                                   dataPtr + imgWidth * imgHeight * imgChannel);
    stbi_image_free(dataPtr);
    return Check();
  }

  bool Check() const {
    return imgWidth > 0 && imgHeight > 0 && imgChannel > 0 &&
           imgData.size() == imgHeight * imgWidth * imgChannel &&
           textureId != 0;
  }

  const uint8_t *GetData() const { return imgData.data(); }

  GLuint GetId() const { return textureId; }

  void Activate() const {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(vao);
  }

  void Write() const {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, imgData.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(imgData)) * imgData.size(),
                 imgData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
  }

private:
  std::string filename;
  GLuint programId;
  GLuint textureId;
  int imgWidth = 0;
  int imgHeight = 0;
  int imgChannel = 0;
  std::vector<uint8_t> imgData;
  std::vector<float> texCoords{1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  GLuint vao, vbo;
};