#include "TextureMappingDemo.h"

TextureMappingApp::TextureMappingApp(WindowInfo info) : GLApp(info) {}

TextureMappingApp::~TextureMappingApp() = default;

void TextureMappingApp::Init() {
    GLApp::Init();

    m_program.AddShader(GL_VERTEX_SHADER, R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)");
    m_program.AddShader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D faceTexture;
uniform sampler2D wallTexture;
uniform int textureMode;

void main() {
    switch (textureMode) {
        case 1:
            FragColor = texture(wallTexture, TexCoord);
            break;
        case 2:
            FragColor = texture(faceTexture, TexCoord);
            break;
        case 3:
            FragColor = mix(texture(wallTexture, TexCoord), texture(faceTexture, TexCoord), 0.5f);
            break;
        default:
            FragColor = vec4(1.0f); // 默认颜色
    }
}

)"));

    if (!m_program.Init()) {
        LOGE("program init failed");
        return;
    }
    m_triangle_mesh = std::make_unique<Mesh>();
    std::vector<glm::vec3> triangle_vertices{{-0.5, -0.5, 0}, {+0.5, -0.5, 0}, {+0.0, +0.5, 0}};
    m_triangle_mesh->AddBuffer(0, BufferType::VERTEX_3D, triangle_vertices);
    std::vector<glm::vec2> triangle_uv{{0, 0}, {1, 0}, {0.5, 1}};
    m_triangle_mesh->AddBuffer(1, BufferType::TEX_COORD, triangle_uv);
    m_triangle_mesh->SetIndices({0, 1, 2});
    Texture faceTexture("faceTexture", 1);
    faceTexture.SetTextureId(Model::TextureFromFile("awesomeface.png", PROJECT_DIR "texture"));
    m_triangle_mesh->AddTexture(faceTexture);
    Texture wallTexture("wallTexture", 2);
    wallTexture.SetTextureId(Model::TextureFromFile("wall.jpg", PROJECT_DIR "texture"));
    m_triangle_mesh->AddTexture(wallTexture);
    m_triangle_mesh->Setup();
}

void TextureMappingApp::OnDrawFrame() {
    GLApp::OnDrawFrame();

    m_program.Use();
    m_program.SetUniform("textureMode", m_texture_mode);
    if (m_triangle_mesh) {
        m_triangle_mesh->Draw(m_program);
    }
}

void TextureMappingApp::ProcessInput(GLFWwindow *win) {
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) {
        m_texture_mode = 3;
        return;
    }
    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) {
        m_texture_mode = 1;
        return;
    }
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) {
        m_texture_mode = 2;
        return;
    }
    GLApp::ProcessInput(win);
}

int main() {
    WindowInfo info;
    info.width = 800 * 2;
    info.height = 600 * 2;
    info.title = GetFileName(__FILE__);
    auto app = std::make_unique<TextureMappingApp>(info);
    app->Init();
    app->Run();
    return 0;
}
