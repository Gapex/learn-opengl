#pragma once

#include "GLApp.h"
#include "Program.h"
#include "Model.h"

class HelloTriangleApp : public GLApp {
  public:
    HelloTriangleApp(WindowInfo info);
    ~HelloTriangleApp() override;
    void Init() override;
    void OnDrawFrame() override;

  private:
    Program m_program{};
    std::unique_ptr<Mesh> m_triangle_mesh{};
};