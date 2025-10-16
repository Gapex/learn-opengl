#include <iostream>
#include <thread>
#include <unordered_map>

#include "GLApp.h"
#include "log.h"
#include "stb_image.h"

void GLApp::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    GLApp *app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->window_info.width = width;
        app->window_info.height = height;
        glViewport(0, 0, width, height);
    }
}

void GLApp::cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos) {
    GLApp *app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
    if (app) {
        if (app->firstMouse) {
            app->lastX = x_pos;
            app->lastY = y_pos;
            app->firstMouse = false;
        } else {
            app->camera.ProcessMouseMovement(x_pos - app->lastX, y_pos - app->lastY);
            app->lastX = x_pos;
            app->lastY = y_pos;
        }
    }
}

void GLApp::scroll_callback(GLFWwindow *window, double, double yoffset) {
    GLApp *app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
    if (app) {
        app->camera.ProcessMouseScroll(yoffset);
    }
}

GLApp::GLApp(const WindowInfo &window_info)
    : window_info(window_info), camera(glm::vec3{1, 1, 3}, glm::vec3(0, 1, 0), YAW, PITCH) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(window_info.width, window_info.height, window_info.title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        LOGE("create GLFW window failed");
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    // glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOGE("Failed to load GLAD");
        return;
    }
}

GLApp::~GLApp() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLApp::ProcessInput(GLFWwindow *win) {
    static std::unordered_map<int, Camera_Movement> keyMap{
        {GLFW_KEY_W, FORWARD},  {GLFW_KEY_A, LEFT}, {GLFW_KEY_D, RIGHT},
        {GLFW_KEY_S, BACKWARD}, {GLFW_KEY_E, UP},   {GLFW_KEY_C, DOWN},
    };
    static std::unordered_map<int, std::pair<float, float>> mouseMap{
        {GLFW_KEY_UP, {0.0f, -10.0f}},
        {GLFW_KEY_DOWN, {0.0f, +10.0f}},
        {GLFW_KEY_LEFT, {-10.0f, 0.0f}},
        {GLFW_KEY_RIGHT, {+10.0f, 0.0f}},
    };
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }
    for (auto [key, move] : keyMap) {
        if (glfwGetKey(win, key) == GLFW_PRESS) {
            camera.ProcessKeyboard(move, timeDelta);
        }
    }
    for (auto &[key, move] : mouseMap) {
        if (glfwGetKey(win, key) == GLFW_PRESS) {
            camera.ProcessMouseMovement(move.first, move.second);
        }
    }
}

void GLApp::Run() {
    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);
        OnDrawFrame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void GLApp::Init() {
    int available_vertex_cnt = -1;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &available_vertex_cnt);
    LOGD("available vertices count: %d", available_vertex_cnt);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    lastTime = glfwGetTime();

}

glm::mat4 GLApp::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(camera.zoom), window_info.width * 1.0f / window_info.height, 0.1f, 1000.0f);
}

void GLApp::OnDrawFrame() {
    const auto currentTime = static_cast<float>(glfwGetTime());
    timeDelta = currentTime - lastTime;
    lastTime = currentTime;
    glClearColor(color_bg.x, color_bg.y, color_bg.z, color_bg.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
