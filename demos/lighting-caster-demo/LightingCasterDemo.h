#pragma once

#include "GLApp.h"
#include "Model.h"
#include "Program.h"

#define CLASS_NAME LightingCasterDemo

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class CLASS_NAME : public GLApp {
  public:
    CLASS_NAME(WindowInfo info);
    ~CLASS_NAME() override;
    void Init() override;
    void OnDrawFrame() override;
    void ProcessInput(GLFWwindow *win) override;

  private:
    Program m_light_program{}, m_cube_program{};
    std::unique_ptr<Model> m_cube_model{};
    glm::vec3 m_light_color{1.0, 1.0, 1.0};
    const glm::vec3 m_cube_color{1, 0.3, 0.2};
    glm::vec4 m_light_position{2, 0, 5, 0};
    bool m_flash_light_enable{false};
    std::vector<glm::vec3> cubePositions = {glm::vec3(0.0f, 0.0f, -5.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                            glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                            glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                            glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                            glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    bool m_showImGuiWindow{true};
};
