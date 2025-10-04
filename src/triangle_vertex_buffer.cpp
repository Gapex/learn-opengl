//
// Created by gaoyunpeng on 2025/10/4.
//

#include "triangle_vertex_buffer.h"

TriangleVertexBuffer::TriangleVertexBuffer(GLuint program_id) : VertexBuffer(program_id) {}

TriangleVertexBuffer::~TriangleVertexBuffer() {}

void TriangleVertexBuffer::Write() {
    Activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), static_cast<GLvoid *>(data.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(),
                 static_cast<GLvoid *>(indices.data()), GL_STATIC_DRAW);
    glUniform1f(glGetUniformLocation(program, "timestamp"), timestamp);
}

void TriangleVertexBuffer::Draw() const {
    Activate();
    if (!indices.empty()) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    }
}