#pragma once

#include <memory>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "camera.h"
#include "vertex_buffer.hpp"
#include "program.hpp"
#include "window_info.h"
#include "data_types.h"

class GLApp {
  public:
    GLApp(WindowInfo window_info);
    ~GLApp();
    void run();

  private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos);

    static void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);

    void ProcessInput(GLFWwindow *win);

    void UpdateClock();

    static void LoadTexture(const GLuint &texture_id, const char *filename, GLuint img_format);

    void Init();

    void onDrawFrame();

    void InitImGui();

    void onDrawImGuiFrame();

  private:
    const glm::vec4 color_bg{0.3f, 0.3f, 0.3f, 1.0f};
    size_t g_clock = 0;
    size_t frame_freq = 60;
    WindowInfo window_info{};
    GLFWwindow *window{};

    std::vector<glm::vec3> cubePositions{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                         glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                         glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                         glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                         glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    glm::vec3 lightPosition = glm::vec3(5.0f, 5.0f, -5.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Program cube_program{}, coord_program{}, light_program{}, bag_program{};
    double timeDelta{}, lastTime{};
    bool firstMouse = true;
    double lastX{}, lastY{};
    std::unique_ptr<Model> bagModel{}, cubeModel{};
    Camera camera;
};
