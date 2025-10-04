//
// Created by gaoyunpeng on 2025/10/4.
//

#ifndef LEARN_OPENGL_LINE_VERTEX_BUFFER_H
#define LEARN_OPENGL_LINE_VERTEX_BUFFER_H

#include "vertex_buffer.hpp"

class LineVertexBuffer : public VertexBuffer {
  public:
    LineVertexBuffer(GLuint program_id, float line_width);
    ~LineVertexBuffer();

    void Write() override;
    void Draw() const override;
private:
    const float line_width{};
};

#endif // LEARN_OPENGL_LINE_VERTEX_BUFFER_H
