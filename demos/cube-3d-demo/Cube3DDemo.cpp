#include "Cube3DDemo.h"
#include "Model.h"

CLASS_NAME::CLASS_NAME(WindowInfo info) : GLApp(info) {}

CLASS_NAME::~CLASS_NAME() = default;

void CLASS_NAME::Init() {
    GLApp::Init();

    m_program.AddShader(GL_VERTEX_SHADER, R"(
#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_tex_coord;

uniform mat4 model, view, projection;

out vec2 tex_coord;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
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
    //frag_color = vec4(tex_coord.s, 0, 0, 1);
}
)");
    if (!m_program.Init()) {
        return;
    }
    std::vector<glm::vec3> cube_vertices = {
        {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f},  {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},  {0.5f, 0.5f, -0.5f},  {-0.5f, 0.5f, -0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},   {0.5f, 0.5f, 0.5f},   {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},  {-0.5f, 0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},    {0.5f, 0.5f, -0.5f},  {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f},   {-0.5f, -0.5f, 0.5f}};

    std::vector<glm::vec2> cube_uvs = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    std::vector<unsigned int> cube_indices = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                                              12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

    m_square_mesh.AddBuffer(0, BufferType::VERTEX_3D, cube_vertices);
    m_square_mesh.AddBuffer(1, BufferType::TEX_COORD, cube_uvs);
    m_square_mesh.SetIndices(cube_indices);
    Texture faceTexture("face_texture", 1);
    faceTexture.SetTextureId(Model::TextureFromFile("awesomeface.png", PROJECT_DIR "texture"));
    m_square_mesh.AddTexture(faceTexture);
    m_square_mesh.Setup();
    CheckGLError();
}

static std::vector<glm::vec3> positions{
    glm::vec3( 0.0f,  0.0f,  -5.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

void CLASS_NAME::OnDrawFrame() {
    GLApp::OnDrawFrame();

    m_program.Use();
    m_program.SetUniform("view", camera.GetViewMatrix());
    m_program.SetUniform("projection", GetProjectionMatrix());
    for (int i = 0; i<positions.size(); i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_program.SetUniform("model", model);
        m_square_mesh.Draw(m_program);
    }
    CheckGLError();
}

int main() {
    WindowInfo info;
    info.height = 600 * 2;
    info.width = 600 * 2;
    info.title = GetFileName(__FILE__);
    auto app = std::make_unique<CLASS_NAME>(info);
    app->Init();
    app->Run();
}
