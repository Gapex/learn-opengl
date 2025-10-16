#pragma once

#include "GLApp.h"
#include "Program.h"
#include "Model.h"

class TextureMappingApp : public GLApp {
  public:
    TextureMappingApp(WindowInfo info);
    ~TextureMappingApp() override;
    void Init() override;
    void OnDrawFrame() override;

  private:
    Program m_program{};
    std::unique_ptr<Mesh> m_triangle_mesh{};
};