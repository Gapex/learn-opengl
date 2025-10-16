#include "ColorChangeTriangle.h"

ColorChangeTriangle::ColorChangeTriangle(WindowInfo info) : GLApp(info) {}

ColorChangeTriangle::~ColorChangeTriangle() = default;

void ColorChangeTriangle::Init() {
    GLApp::Init();

    m_program.AddShader(GL_VERTEX_SHADER, R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)");
    m_program.AddShader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, R"(
#version 330 core

out vec4 FragColor;

uniform float time;

void main() {
    FragColor = vec4(sin(time) + 1.0, cos(time) + 1.0, time, 1.0);
}
)"));

    if (!m_program.Init()) {
        LOGE("program init failed");
        return;
    }
    m_triangle_mesh = std::make_unique<Mesh>();
    std::vector<glm::vec3> triangle_vertices{{-0.5, -0.5, 0}, {+0.5, -0.5, 0}, {+0.0, +0.5, 0}};
    m_triangle_mesh->AddBuffer(0, BufferType::VERTEX_3D, triangle_vertices);
    m_triangle_mesh->SetIndices({0, 1, 2});
    m_triangle_mesh->Setup();
}

void ColorChangeTriangle::OnDrawFrame() {
    GLApp::OnDrawFrame();

    m_program.Use();
    m_program.SetUniform("time", float(glfwGetTime()));
    if (m_triangle_mesh) {
        m_triangle_mesh->Draw(m_program);
    }
}

int main() {
    WindowInfo info;
    info.width = 800 * 2;
    info.height = 600 * 2;
    info.title = GetFileName(__FILE__);
    auto app = std::make_unique<ColorChangeTriangle>(info);
    app->Init();
    app->Run();
    return 0;
}
