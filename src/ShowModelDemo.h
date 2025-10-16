#ifndef LEARN_OPENGL_SHOWMODELDEMO_H
#define LEARN_OPENGL_SHOWMODELDEMO_H

#include "GLApp.h"

class ShowModelDemo : public GLApp {
  public:
    explicit ShowModelDemo(WindowInfo info);
    ~ShowModelDemo() override = default;
    void Init() override;
    void OnDrawFrame() override;

  private:
    std::vector<glm::vec3> cubePositions{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 0.0f, -15.0f),
                                         glm::vec3(-1.5f, -0.0f, -2.5f), glm::vec3(-3.8f, 0.0f, -12.3f),
                                         glm::vec3(4.4f, -0.0f, -3.5f),  glm::vec3(-1.7f, 0.0f, -7.5f),
                                         glm::vec3(1.3f, -0.0f, 2.5f),   glm::vec3(5.32f, 0.0f, +7.5f),
                                         glm::vec3(2.5f, 0.0f, -1.5f),   glm::vec3(-1.3f, 0.0f, -1.5f)};
    glm::vec3 lightPosition = glm::vec3(5.0f, 5.0f, -5.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Program cube_program{}, coord_program{}, light_program{}, bag_program{}, plane_program{};
    std::unique_ptr<Model> bagModel{}, cubeModel{}, planeModel{}, transparent_window{};
    const float planeScale = 100.0f;
    const float planeHeight = -1.001;
};

#endif // LEARN_OPENGL_SHOWMODELDEMO_H
