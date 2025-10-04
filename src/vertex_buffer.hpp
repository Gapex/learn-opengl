#pragma once

#include "program.hpp"
#include <glad/glad.h>
#include <vector>

class VertexBuffer {
  public:
    VertexBuffer(unsigned int program);
    virtual ~VertexBuffer() = default;

    void Activate() const { glBindVertexArray(vao); }

    virtual void Write() = 0;

    void AddVertexes(const std::vector<float> &newData);

    void AddIndices(const std::vector<unsigned int> newIndices);

    void Clear();

    virtual void Draw() const = 0;

    void SetTime(float newTimeStamp) { timestamp = newTimeStamp; }

    void SetVertexCnt(const GLuint vertex_cnt) { this->vertexCnt = vertex_cnt; }

  protected:
    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    unsigned int program{};
    std::vector<float> data{};
    GLuint vertexCnt{};
    std::vector<unsigned int> indices{};
    float timestamp{};
    GLint timestampUniformLocation{};
};
