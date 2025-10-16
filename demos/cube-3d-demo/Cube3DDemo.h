#pragma once

#include "GLApp.h"
#include "Mesh.h"
#include "Program.h"

#define CLASS_NAME Cube3DDemo

class CLASS_NAME : public GLApp {
  public:
    CLASS_NAME(WindowInfo info);
    ~CLASS_NAME() override;
    void Init() override;
    void OnDrawFrame() override;

  private:
    Program m_program{};
    Mesh m_square_mesh{};
};
