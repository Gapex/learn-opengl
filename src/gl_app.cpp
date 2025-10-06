#include "gl_app.h"

#include "line_vertex_buffer.h"
#include "log.h"
#include "stb_image.h"
#include "triangle_vertex_buffer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <chrono>
#include <iostream>
#include <map>
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

    Init();
    InitImGui();
}

void GLApp::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

GLApp::~GLApp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

void GLApp::UpdateClock() {
    g_clock = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                  .count();
}

void GLApp::run() {
    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);
        UpdateClock();
        onDrawImGuiFrame();
        onDrawFrame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);
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
    bag_program.Append(std::make_shared<Shader>(GL_VERTEX_SHADER, PROJECT_DIR "glsl/bag.vertex.glsl"));
    bag_program.Append(std::make_shared<Shader>(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/bag.frag.glsl"));
    plane_program.Append(std::make_shared<Shader>(GL_VERTEX_SHADER, PROJECT_DIR "glsl/depth_test.vertex.glsl"));
    plane_program.Append(std::make_shared<Shader>(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/depth_test.frag.glsl"));

    if (!plane_program.Init()) {
        LOGE("Failed to initialize depth test program");
        return;
    }
    if (!bag_program.Init()) {
        LOGE("Failed to initialize bag program");
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bagModel = std::make_unique<FileModel>(PROJECT_DIR "assets/backpack/backpack.obj");
    cubeModel = std::make_unique<FileModel>(PROJECT_DIR "assets/cube/cube.obj");
    Texture wallTexture;
    wallTexture.id = Model::TextureFromFile("marble.jpg", PROJECT_DIR "texture/");
    wallTexture.type = "texture_diffuse";
    cubeModel->meshes.front().textures.emplace_back(wallTexture);

    planeModel = std::make_unique<Model>();
    planeModel->meshes.emplace_back();
    Mesh &planeMesh = planeModel->meshes.back();
    std::vector<Vertex> planeVertices(6);
    planeVertices[0].position = glm::vec3(1.0f, 0, 1.0f);
    planeVertices[0].texCoords = glm::vec2(0.0f, 0.0f);
    planeVertices[1].position = glm::vec3(-1.0f, 0, 1.0f);
    planeVertices[1].texCoords = glm::vec2(1.0f, 0.0f);
    planeVertices[2].position = glm::vec3(-1.0f, 0, -1.0f);
    planeVertices[2].texCoords = glm::vec2(1, 1.0f);
    planeVertices[3].position = glm::vec3(1.0f, 0, 1.0f);
    planeVertices[3].texCoords = glm::vec2(0.0f, 0.0f);
    planeVertices[4].position = glm::vec3(-1.0f, 0, -1.0f);
    planeVertices[4].texCoords = glm::vec2(1, 1.0f);
    planeVertices[5].position = glm::vec3(1.0f, 0, -1.0f);
    planeVertices[5].texCoords = glm::vec2(0.0f, 1.0f);
    planeMesh.vertices = planeVertices;
    for (auto &vertex : planeMesh.vertices) {
        vertex.texCoords *= planeScale;
        vertex.position.y = planeHeight;
    }
    planeMesh.indices = {0, 1, 2, 3, 4, 5};
    Texture planeTexture;
    planeTexture.id = Model::TextureFromFile("metal.png", PROJECT_DIR "texture/");
    planeTexture.type = "texture_diffuse";
    planeMesh.textures.emplace_back(planeTexture);
    planeMesh.Setup();
    CheckGLError();
    lastTime = glfwGetTime();

    transparent_window = std::make_unique<Model>();
    transparent_window->meshes.emplace_back();
    Mesh &transparent_window_mesh = transparent_window->meshes.back();
    transparent_window_mesh.vertices = planeVertices;
    transparent_window_mesh.indices = {0, 1, 2, 3, 4, 5};
    Texture transparent_window_texture;
    transparent_window_texture.id = Model::TextureFromFile("transparent_window.png", PROJECT_DIR "texture/", GL_CLAMP_TO_EDGE);
    transparent_window_texture.type = "texture_diffuse";
    transparent_window_mesh.textures.emplace_back(transparent_window_texture);
    transparent_window_mesh.Setup();
}

void GLApp::onDrawImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Hello Imgui");
        ImGui::Text("Frame Rate: %d Hz", int(ImGui::GetIO().Framerate));
        ImGui::End();
    }
    ImGui::Render();
}

void GLApp::onDrawFrame() {
    const float currentTime = static_cast<float>(glfwGetTime());
    timeDelta = currentTime - lastTime;
    lastTime = currentTime;
    glClearColor(color_bg.x, color_bg.y, color_bg.z, color_bg.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 projection =
        glm::perspective(glm::radians(camera.zoom), window_info.width * 1.0f / window_info.height, 0.1f, 1000.0f);


    bag_program.Activate();
    bag_program.SetMat4("viewMat", camera.GetViewMatrix());
    bag_program.SetMat4("projMat", projection);
    // if (bagModel) {
    //     bag_program.SetMat4("modelMat", glm::mat4(1.0f));
    //     bagModel->Draw(bag_program);
    // }
    if (cubeModel) {
        for (size_t i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions.at(i) * 2.0f);
            bag_program.SetMat4("modelMat", model);
            cubeModel->Draw(bag_program);
        }
    }

    plane_program.Activate();
    plane_program.SetMat4("projMat", projection);
    plane_program.SetMat4("viewMat", camera.GetViewMatrix());
    if (planeModel) {
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(planeScale, 1.0f, planeScale));
        plane_program.SetMat4("modelMat", model);
        planeModel->Draw(plane_program);
    }
    if (transparent_window) {
        std::map<float, glm::mat4> sorted_transparent_windows;
        for (size_t i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            glm::vec3 window_pos = cubePositions[i] * 2.0f;
            window_pos.y += 0.01f;
            window_pos.z += 1.5f;
            window_pos.x += glm::sin(glfwGetTime() * ( i + 1));
            model = glm::translate(model, window_pos);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            sorted_transparent_windows[glm::length(camera.position - window_pos)] = model;
        }
        for (auto iter =  sorted_transparent_windows.rbegin(); iter != sorted_transparent_windows.rend(); iter++) {
            bag_program.SetMat4("modelMat", iter->second);
            transparent_window->Draw(plane_program);
        }
    }
}
