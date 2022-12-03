#pragma once

#include <glad/glad.h>
#include <vector>

#include "VecN.hpp"

class VertexBuffer {
public:
  VertexBuffer(GLuint programId) : programId(programId) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    timestampUniformLocation = glGetUniformLocation(programId, "timestamp");
  }

  void Activate() const { glBindVertexArray(vao); }

  void Write() const {
    Activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(),
                 (GLvoid *)(data.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(decltype(indices)::value_type) * indices.size(),
                 (GLvoid *)(indices.data()), GL_STATIC_DRAW);

    glUniform1f(timestampUniformLocation, timestamp);
  }

  void AddVertexes(const std::vector<float> &newData) {
    data.reserve(data.size() + newData.size());
    for (auto val : newData) {
      data.push_back(val);
    }
  }

  void AddIndices(const std::vector<unsigned int> newIndices) {
    indices.reserve(indices.size() + newIndices.size());
    for (auto idxVal : newIndices) {
      indices.push_back(idxVal);
    }
  }

  void Clear() {
    data.clear();
    indices.clear();
    timestamp = 0;
  }

  void Draw() const {
    Activate();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
  }

  void SetTime(float newTimeStamp) { timestamp = newTimeStamp; }

private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLuint programId;
  std::vector<float> data;
  std::vector<unsigned int> indices;
  float timestamp;
  GLint timestampUniformLocation;
};