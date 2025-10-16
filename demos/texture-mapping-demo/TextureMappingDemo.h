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
    void ProcessInput(GLFWwindow *win) override;;

  private:
    Program m_program{};
    int m_texture_mode{3};
    std::unique_ptr<Mesh> m_triangle_mesh{};
};