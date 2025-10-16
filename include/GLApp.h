#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "WindowInfo.h"

class GLApp {
  public:
    explicit GLApp(const WindowInfo &window_info);
    virtual ~GLApp();
    void Run();
    virtual void Init();

  protected:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos);

    static void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);

    virtual void ProcessInput(GLFWwindow *win);

    virtual void OnDrawFrame();

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

    const glm::vec4 color_bg{0.1f, 0.1f, 0.1f, 1.0f};
    size_t frame_freq = 60;
    WindowInfo window_info{};
    GLFWwindow *window{};

    double timeDelta{}, lastTime{};
    bool firstMouse = true;
    double lastX{}, lastY{};
    Camera camera;
};
