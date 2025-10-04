//
// Created by gaoyunpeng on 2025/10/4.
//

#ifndef LEARN_OPENGL_TRIANGLE_VERTEX_BUFFER_H
#define LEARN_OPENGL_TRIANGLE_VERTEX_BUFFER_H
#include "vertex_buffer.hpp"

class TriangleVertexBuffer : public VertexBuffer {
  public:
    TriangleVertexBuffer(GLuint program_id);
    ~TriangleVertexBuffer() override;

    void Write() override;
    void Draw() const override;
};

#endif // LEARN_OPENGL_TRIANGLE_VERTEX_BUFFER_H
