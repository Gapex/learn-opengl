#pragma once

#include "program.hpp"
#include <glad/glad.h>
#include <vector>

class VertexBuffer {
  public:
    VertexBuffer(unsigned int program) : program(program) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    void Activate() const { glBindVertexArray(vao); }

    void Write() {
        Activate();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), (GLvoid *)(data.data()), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(),
                     (GLvoid *)(indices.data()), GL_STATIC_DRAW);
        glUniform1f(glGetUniformLocation(program, "timestamp"), timestamp);
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
        if (!indices.empty()) {
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
        }
    }

    void SetTime(float newTimeStamp) { timestamp = newTimeStamp; }

    void SetVertexCnt(GLuint vertex_cnt) { this->vertexCnt = vertex_cnt; }

  private:
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
