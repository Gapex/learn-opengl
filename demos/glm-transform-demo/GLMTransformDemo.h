#pragma once

#include "GLApp.h"
#include "Mesh.h"
#include "Program.h"

class GLMTransformDemo : public GLApp {
  public:
    GLMTransformDemo(WindowInfo info);
    ~GLMTransformDemo() override;
    void Init() override;
    void OnDrawFrame() override;

  private:
    Program m_program{};
    Mesh m_square_mesh{};
};
