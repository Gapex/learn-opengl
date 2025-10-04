//
// Created by gaoyunpeng on 2025/10/4.
//
#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(unsigned int program) : program(program) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void VertexBuffer::AddVertexes(const std::vector<float> &newData) {
    data.reserve(data.size() + newData.size());
    for (auto val : newData) {
        data.push_back(val);
    }
}

void VertexBuffer::AddIndices(const std::vector<unsigned int> newIndices) {
    indices.reserve(indices.size() + newIndices.size());
    for (auto idxVal : newIndices) {
        indices.push_back(idxVal);
    }
}

void VertexBuffer::Clear() {
    data.clear();
    indices.clear();
    timestamp = 0;
}


