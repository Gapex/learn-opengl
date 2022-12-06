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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

static float screenWidth = 1;
static float screenHeight = 1;
static Color4f color_bg(0.3, 0.3, .3, 1);
static size_t g_clock = 0;
static size_t frameFreq = 120;
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
static GLuint ourTexture, ourTexture2;
glm::mat4 view(1.0);
glm::vec3 viewPos(0.0f, 0.0f, -3.0f);

void processInput(GLFWwindow *win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, true);
  }
  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
    viewPos.z -= 0.04;
  } else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
    viewPos.x -= 0.04;
  } else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
    viewPos.x += 0.04;
  } else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
    viewPos.z += 0.04;
  }
}

void updateClock() {
  g_clock = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
}

void loadTexture(GLuint &textureId, const char *filename) {
  glBindTexture(GL_TEXTURE_2D, textureId);
  //设置纹理环绕方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //设置采样方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //加载纹理
  int width, height, channel;
  stbi_set_flip_vertically_on_load(true);
  const unsigned char *texData =
      stbi_load(filename, &width, &height, &channel, 0);
  if (!texData) {
    std::cerr << "load texture data failed" << std::endl;
    exit(-1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, texData);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void LoadTexture(GLuint &textureId, const char *filename, GLuint imgFormat) {
  glBindTexture(GL_TEXTURE_2D, textureId);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, width, height, 0, imgFormat,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
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
  glActiveTexture(GL_TEXTURE0);
  LoadTexture(ourTexture, "../texture/wall.jpg", GL_RGB);
  glGenTextures(1, &ourTexture2);
  glActiveTexture(GL_TEXTURE1);
  LoadTexture(ourTexture2, "../texture/awesomeface.png", GL_RGBA);
  // bind textures on corresponding texture units
  view = glm::translate(view, viewPos);
}

void onDrawFrame() {
  // std::cout << "on draw frame..." << g_clock << std::endl;
  glClearColor(color_bg.r(), color_bg.g(), color_bg.b(), color_bg.a());
  glClear(GL_COLOR_BUFFER_BIT);

  program.Activate();
  if (!vertexBuf) {
    vertexBuf = std::make_shared<VertexBuffer>(program.GetId());
  }
  glm::mat4 model(1.0);
  model = glm::rotate(model, glm::radians(-55.0f * sinf(glfwGetTime())),
                      glm::vec3(1.0f, 0.0f, .0f));

  view = glm::mat4(1.0);
  view = glm::translate(view, viewPos);

  glm::mat4 projection(1.0f);
  projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight,
                                0.1f, 100.0f);
  glm::mat4 mvp = projection * view * model;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  vertexBuf->Clear();
  vertexBuf->AddVertexes({
      // positions        // texture coords
      +0.5f, +0.5f, 0.0f, 0.0f, 1.0f, // top left
      +0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, +0.5f, 0.0f, 1.0f, 1.0f  // top right
  });
  vertexBuf->AddIndices({0, 1, 3, 1, 2, 3});
  vertexBuf->SetTime(glfwGetTime());
  vertexBuf->Write();
  glUniform1i(glGetUniformLocation(program.GetId(), "ourTexture"), 0);
  glUniform1i(glGetUniformLocation(program.GetId(), "ourTexture2"), 1);
  glad_glUniformMatrix4fv(glGetUniformLocation(program.GetId(), "trans"), 1,
                          GL_FALSE, glm::value_ptr(mvp));
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
                                   screenWidth = width;
                                   screenHeight = height;
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