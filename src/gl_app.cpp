#include "gl_app.h"

#include "line_vertex_buffer.h"
#include "log.h"
#include "stb_image.h"
#include "triangle_vertex_buffer.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <unordered_map>

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

GLApp::GLApp(WindowInfo window_info)
    : window_info(window_info), camera(glm::vec3{0, 0, 3}, glm::vec3(0, 1, 0), YAW, PITCH) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    win = glfwCreateWindow(window_info.width, window_info.height, window_info.title.c_str(), nullptr, nullptr);
    if (win == nullptr) {
        LOGE("create GLFW window failed");
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(win, this);
    glfwMakeContextCurrent(win);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetCursorPosCallback(win, cursor_position_callback);

    glfwSetScrollCallback(win, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOGE("Failed to load GLAD");
        return;
    }

    Init();
}

GLApp::~GLApp() {
    glfwDestroyWindow(win);
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

void GLApp::UpdateClock() {
    g_clock = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                  .count();
}

void GLApp::run() {
    while (!glfwWindowShouldClose(win)) {
        ProcessInput(win);
        UpdateClock();
        onDrawFrame();
        glfwPollEvents();
        glfwSwapBuffers(win);
    }
}

void GLApp::LoadTexture(const GLuint &texture_id, const char *filename, GLuint img_format) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, img_format, width, height, 0, img_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOGE("Failed to load texture: %s", filename);
    }
    stbi_image_free(data);
}

void GLApp::Init() {
    int available_vertex_cnt = -1;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &available_vertex_cnt);
    LOGD("available vertices count: %d", available_vertex_cnt);
    std::shared_ptr<Shader> vertexShader(new Shader(GL_VERTEX_SHADER, PROJECT_DIR "glsl/vertex.glsl"));
    if (!vertexShader->Init()) {
        LOGE("Failed to initialize vertex shader");
        return;
    }
    cube_program.Append(vertexShader);

    std::shared_ptr<Shader> frag_shader(new Shader(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/fragment.glsl"));
    if (!frag_shader->Init()) {
        LOGE("Failed to initialize fragment shader");
        return;
    }
    cube_program.Append(frag_shader);

    coord_program.Append(std::make_shared<Shader>(GL_VERTEX_SHADER, PROJECT_DIR "glsl/line.vertex.glsl"));
    coord_program.Append(std::make_shared<Shader>(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/line.frag.glsl"));

    if (!cube_program.Init()) {
        LOGE("Failed to initialize cube program");
        return;
    }
    if (!coord_program.Init()) {
        LOGE("Failed to initialize line program");
        return;
    }
    glGenTextures(1, &ourTexture);
    glActiveTexture(GL_TEXTURE0);
    LoadTexture(ourTexture, PROJECT_DIR "texture/container.jpg", GL_RGB);
    glGenTextures(1, &ourTexture2);
    glActiveTexture(GL_TEXTURE1);
    LoadTexture(ourTexture2, PROJECT_DIR "texture/awesomeface.png", GL_RGBA);
    glEnable(GL_DEPTH_TEST);
    lastTime = glfwGetTime();
}

void GLApp::onDrawFrame() {
    const float currentTime = static_cast<float>(glfwGetTime());
    timeDelta = currentTime - lastTime;
    lastTime = currentTime;
    glClearColor(color_bg.x, color_bg.y, color_bg.z, color_bg.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!cube_vertex_buffer) {
        cube_vertex_buffer = std::make_shared<TriangleVertexBuffer>(cube_program.GetId());
    }
    if (!coordinates_vertex_buffer) {
        coordinates_vertex_buffer = std::make_shared<LineVertexBuffer>(coord_program.GetId(), 3.0f);
    }

    const glm::mat4 projection =
        glm::perspective(glm::radians(camera.zoom), window_info.width * 1.0f / window_info.height, 0.1f, 1000.0f);
    const glm::mat4 view = camera.GetViewMatrix();

    coord_program.Activate();
    coordinates_vertex_buffer->Clear();
    coordinates_vertex_buffer->AddVertexes(coordinates);
    coordinates_vertex_buffer->SetTime(currentTime);
    coordinates_vertex_buffer->SetVertexCnt(coordinates.size());
    coordinates_vertex_buffer->Write();
    coord_program.SetMat4("trans", projection * view);
    coordinates_vertex_buffer->Draw();

    cube_program.Activate();
    cube_vertex_buffer->Clear();
    cube_vertex_buffer->AddVertexes(cube);
    cube_vertex_buffer->SetTime(currentTime);
    cube_vertex_buffer->SetVertexCnt(36);
    cube_vertex_buffer->Write();
    cube_program.SetInt("boxTexture", 0);
    cube_program.SetInt("faceTexture", 1);

    for (size_t i = 0; i < cubePositions.size(); i++) {
        glm::mat4 model(1.0f);
        float angle = 20.0f * static_cast<float>(i);
        model = glm::translate(model, cubePositions.at(i));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glm::mat4 mvp = projection * view * model;
        cube_program.SetMat4("trans", mvp);
        cube_vertex_buffer->Draw();
    }
}
