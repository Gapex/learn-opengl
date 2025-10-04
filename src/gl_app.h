#pragma once

#include <memory>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "camera.h"
#include "vertex_buffer.hpp"
#include "program.hpp"
#include "window_info.h"

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

  private:
    const glm::vec4 color_bg{0.3f, 0.3f, 0.3f, 1.0f};
    size_t g_clock = 0;
    size_t frame_freq = 60;
    WindowInfo window_info{};
    GLFWwindow *win{};

    std::vector<float> cube{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    std::vector<glm::vec3> cubePositions{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                         glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                         glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                         glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                         glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    std::vector<float> coordinates{
        -1.0f,    0.0f,     0.0f,     1.0f, 0.0f, 0.0f, 0.0f, // X-axis, red
        +1000.0f, 0.0f,     0.0f,     1.0f, 0.0f, 0.0f, 1.0f, // X-axis, red

        0.0f,     -1.0f,    0.0f,     0.0f, 1.0f, 0.0f, 0.0f, // Y-axis, green
        0.0f,     +1000.0f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, // Y-axis, green

        0.0f,     0.0f,     -1.0f,    0.0f, 0.0f, 1.0f, 0.0f, // Z-axis, blue
        0.0f,     0.0f,     +1000.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Z-axis, blue
    };
    Program cube_program{}, coord_program{};
    std::shared_ptr<VertexBuffer> cube_vertex_buffer{};
    std::shared_ptr<VertexBuffer> coordinates_vertex_buffer{};
    GLuint ourTexture{}, ourTexture2{};
    double timeDelta{}, lastTime{};
    bool firstMouse = true;
    double lastX{}, lastY{};
    Camera camera;
};
