#include "GLMTransformDemo.h"

#include "Model.h"

GLMTransformDemo::GLMTransformDemo(WindowInfo info) : GLApp(info) {}

GLMTransformDemo::~GLMTransformDemo() {}

void GLMTransformDemo::Init() {
    GLApp::Init();

    m_program.AddShader(GL_VERTEX_SHADER, R"(
#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_tex_coord;

uniform mat4 transform;

out vec2 tex_coord;

void main() {
    gl_Position = transform * vec4(a_position, 1.0);
    tex_coord = a_tex_coord;
}
)");
    m_program.AddShader(GL_FRAGMENT_SHADER, R"(
#version 410 core

in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D face_texture;

void main() {
    frag_color = texture(face_texture, tex_coord);
}
)");
    if (!m_program.Init()) {
        return;
    }

    m_square_mesh.AddBuffer(0, BufferType::VERTEX_3D,
                            std::vector<glm::vec3>{{-0.5, -0.5, 0}, {0.5, -0.5, 0}, {0.5, 0.5, 0}, {-0.5, 0.5, 0}});
    m_square_mesh.AddBuffer(1, BufferType::TEX_COORD, std::vector<glm::vec2>{{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    m_square_mesh.SetIndices({0, 1, 2, 2, 3, 0});
    Texture faceTexture("face_texture", 1);
    faceTexture.SetTextureId(Model::TextureFromFile("awesomeface.png", PROJECT_DIR "texture"));
    m_square_mesh.AddTexture(faceTexture);
    m_square_mesh.Setup();
    CheckGLError();
}

void GLMTransformDemo::OnDrawFrame() {
    GLApp::OnDrawFrame();

    m_program.Use();
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(glm::sin(float(glfwGetTime())), 0, 0));
    transform = glm::rotate(transform, float(glfwGetTime()), glm::vec3(0, 0, 1));
    m_program.SetUniform("transform", transform);
    m_square_mesh.Draw(m_program);
    CheckGLError();
}

int main() {
    WindowInfo info;
    info.height = 600 * 2;
    info.width = 600 * 2;
    info.title = GetFileName(__FILE__);
    auto app = std::make_unique<GLMTransformDemo>(info);
    app->Init();
    app->Run();
}
