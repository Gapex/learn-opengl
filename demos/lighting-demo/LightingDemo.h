#pragma once

#include "GLApp.h"
#include "Model.h"
#include "Program.h"

#define CLASS_NAME LightingDemo

class CLASS_NAME : public GLApp {
  public:
    CLASS_NAME(WindowInfo info);
    ~CLASS_NAME() override;
    void Init() override;
    void OnDrawFrame() override;

  private:
    Program m_light_program{}, m_cube_program{};
    std::unique_ptr<Model> m_cube_model{};
    glm::vec3 m_light_color{1.0, 1.0, 1.0};
    const glm::vec3 m_cube_color{1, 0.3, 0.2};
    glm::vec3 m_light_position{5, 3, -5};
};
