#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Color.hpp"
#include "Program.hpp"
#include "Size.hpp"
#include "Triangle.hpp"
#include "VertexBuffer.hpp"

static Color4f color_bg(0.3, 0.3, .3, 1);
static size_t g_clock = 0;
static size_t frameFreq = 30;
static Size<size_t> winSize(800, 800);
static Triangle triangle({ Vertex(-.5f, -.5f, 0), Vertex(.5f, -.5f, 0), Vertex(0, .5f, 0) });
static Program program;
static SP<VertexBuffer> vertexBuf;

void processInput(GLFWwindow* win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }
}

void updateClock()
{
    g_clock = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void init()
{
    SP<Shader> vertexShader(new Shader(GL_VERTEX_SHADER, "../glsl/vertex.glsl"));
    if (!vertexShader->Init()) {
        exit(-1);
    }
    program.Append(vertexShader);

    SP<Shader> fragShader(new Shader(GL_FRAGMENT_SHADER, "../glsl/fragment.glsl"));
    if (!fragShader->Init()) {
        exit(-1);
    }
    program.Append(fragShader);

    if (!program.Init()) {
        exit(-1);
    }
}

void onDrawFrame()
{
    std::cout << "on draw frame..." << g_clock << std::endl;
    glClearColor(color_bg.r(), color_bg.g(), color_bg.b(), color_bg.a());
    glClear(GL_COLOR_BUFFER_BIT);
    program.Activate();
    if (!vertexBuf) {
        vertexBuf = std::make_shared<VertexBuffer>();
    }
        float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, dataLen, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* win = glfwCreateWindow(winSize.w, winSize.h, "Learning OpengGL", nullptr, nullptr);
    if (win == nullptr) {
        std::cerr << "create GLFW window failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, [](GLFWwindow* win, int width, int height) {
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