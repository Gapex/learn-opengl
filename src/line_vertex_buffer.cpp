//
// Created by gaoyunpeng on 2025/10/4.
//

#include "line_vertex_buffer.h"

LineVertexBuffer::LineVertexBuffer(GLuint program_id, float line_width)
    : VertexBuffer(program_id), line_width(line_width) {}

LineVertexBuffer::~LineVertexBuffer() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void LineVertexBuffer::Write() {
    Activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), static_cast<GLvoid *>(data.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(),
                 static_cast<GLvoid *>(indices.data()), GL_STATIC_DRAW);
    glUniform1f(glGetUniformLocation(program, "timestamp"), timestamp);
}

void LineVertexBuffer::Draw() const {
    glLineWidth(line_width);
    Activate();
    if (!indices.empty()) {
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_LINES, 0, vertexCnt);
    }
}
