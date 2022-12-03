#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Color.hpp"
#include "Program.hpp"
#include "Size.hpp"
#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include "stb_image.h"

static Color4f color_bg(0.3, 0.3, .3, 1);
static size_t g_clock = 0;
static size_t frameFreq = 60;
static Size<size_t> winSize(800, 800);
static Triangle triangle({Vertex(-.5f, -.5f, 0), Vertex(.5f, -.5f, 0),
                          Vertex(0, .5f, 0)});
static Triangle triangle2({
    Vertex(+.5f, +.5f, .0f), // 右上角
    Vertex(+.5f, -.5f, .0f), // 右下角
    Vertex(-.5f, -.5f, .0f), // 左下角
    Vertex(-.5f, +.5f, .0f)  // 左上角
});
static Program program;
static SP<VertexBuffer> vertexBuf;
static GLuint ourTexture;

void processInput(GLFWwindow *win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, true);
  }
}

void updateClock() {
  g_clock = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
}

void init() {
  int avaiableVertexAttribsCnt = -1;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &avaiableVertexAttribsCnt);
  std::cout << "可用顶点属性数量" << avaiableVertexAttribsCnt << std::endl;
  SP<Shader> vertexShader(new Shader(GL_VERTEX_SHADER, "../glsl/vertex.glsl"));
  if (!vertexShader->Init()) {
    exit(-1);
  }
  program.Append(vertexShader);

  SP<Shader> fragShader(
      new Shader(GL_FRAGMENT_SHADER, "../glsl/fragment.glsl"));
  if (!fragShader->Init()) {
    exit(-1);
  }
  program.Append(fragShader);

  if (!program.Init()) {
    exit(-1);
  }
  glGenTextures(1, &ourTexture);
  glBindTexture(GL_TEXTURE_2D, ourTexture);
  //设置纹理环绕方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //设置采样方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  //加载纹理
  int width, height, channel;
  const unsigned char *texData =
      stbi_load("../texture/wall.jpg", &width, &height, &channel, 0);
  if (!texData) {
    std::cerr << "load texture data failed" << std::endl;
    exit(-1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, texData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free((void *)texData);
}

void onDrawFrame() {
  std::cout << "on draw frame..." << g_clock << std::endl;
  glClearColor(color_bg.r(), color_bg.g(), color_bg.b(), color_bg.a());
  glClear(GL_COLOR_BUFFER_BIT);
  program.Activate();
  if (!vertexBuf) {
    vertexBuf = std::make_shared<VertexBuffer>(program.GetId());
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  vertexBuf->Clear();
  vertexBuf->AddVertexes({
      // positions        // texture coords
      +0.5f, +0.5f, 0.0f, 1.0f, 1.0f, // top right
      +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, +0.5f, 0.0f, 0.0f, 1.0f  // top left
  });
  vertexBuf->AddIndices({0, 1, 2, 0, 2, 3});
  vertexBuf->SetTime(glfwGetTime());
  vertexBuf->Write();
  glBindTexture(GL_TEXTURE_2D, ourTexture);
  vertexBuf->Draw();
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *win = glfwCreateWindow(winSize.w, winSize.h, "Learning OpengGL",
                                     nullptr, nullptr);
  if (win == nullptr) {
    std::cerr << "create GLFW window failed" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win,
                                 [](GLFWwindow *win, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to load GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  init();

  while (!glfwWindowShouldClose(win)) {
    processInput(win);

    updateClock();
    onDrawFrame();

    glfwPollEvents();
    glfwSwapBuffers(win);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frameFreq));
  }
  glfwTerminate();
  return 0;
}