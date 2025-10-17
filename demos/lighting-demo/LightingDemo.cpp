#include "LightingDemo.h"
#include "FileModel.h"

CLASS_NAME::CLASS_NAME(WindowInfo info) : GLApp(info) {}

CLASS_NAME::~CLASS_NAME() = default;

void CLASS_NAME::Init() {
    GLApp::Init();

    m_light_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/lighting-demo/light.vertex.glsl");
    m_light_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/lighting-demo/light.frag.glsl");
    if (!m_light_program.Init()) {
        return;
    }
    m_cube_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/lighting-demo/lighting-cube.vertex.glsl");
    m_cube_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/lighting-demo/lighting-cube.frag.glsl");
    if (!m_cube_program.Init()) {
        return;
    }
    m_cube_model = std::make_unique<FileModel>(PROJECT_DIR "assets/cube/cube.obj");
    CheckGLError();
}

void CLASS_NAME::OnDrawFrame() {
    GLApp::OnDrawFrame();

    // m_light_color = glm::vec3{glm::sin(glfwGetTime()), glm::cos(glfwGetTime()), 0};
    m_light_position.x = 5 * sin(glfwGetTime());
    m_light_position.z = 5 * cos(glfwGetTime());
    m_light_program.Use();
    m_light_program.SetUniform("lightColor", m_light_color);
    m_light_program.SetUniform("view", camera.GetViewMatrix());
    m_light_program.SetUniform("projection", GetProjectionMatrix());
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_light_position);
    model = glm::scale(model, glm::vec3(0.2f));
    m_cube_model->SetModelMatrix(model);
    m_cube_model->Draw(m_light_program);

    m_cube_program.Use();
    m_cube_program.SetUniform("view", camera.GetViewMatrix());
    m_cube_program.SetUniform("projection", GetProjectionMatrix());
    m_cube_program.SetUniform("lightColor", m_light_color);
    m_cube_program.SetUniform("cubeColor", m_cube_color);
    m_cube_program.SetUniform("lightPosition", m_light_position);
    m_cube_program.SetUniform("viewPos", camera.position);
    m_cube_model->SetModelMatrix(glm::mat4(1.0f));
    m_cube_model->Draw(m_cube_program);

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
