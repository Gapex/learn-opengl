#include "LightingCasterDemo.h"
#include "FileModel.h"

CLASS_NAME::CLASS_NAME(WindowInfo info) : GLApp(info) {}

CLASS_NAME::~CLASS_NAME() = default;

void CLASS_NAME::Init() {
    GLApp::Init();

    m_light_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/lighting-caster-demo/light.vertex.glsl");
    m_light_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/lighting-caster-demo/light.frag.glsl");
    if (!m_light_program.Init()) {
        return;
    }
    m_cube_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/lighting-caster-demo/lighting-cube.vertex.glsl");
    m_cube_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/lighting-caster-demo/lighting-cube.frag.glsl");
    if (!m_cube_program.Init()) {
        return;
    }
    m_cube_model = std::make_unique<FileModel>(PROJECT_DIR "assets/cube/cube.obj");
    Texture diffuseTexture("material.diffuse", 0);
    diffuseTexture.SetTextureId(Model::TextureFromFile("container2.png", PROJECT_DIR "texture"));
    Texture specularTexture("material.specular", 1);
    specularTexture.SetTextureId(Model::TextureFromFile("container2_specular.png", PROJECT_DIR "texture"));
    m_cube_model->meshes[0].AddTexture(diffuseTexture);
    m_cube_model->meshes[0].AddTexture(specularTexture);
    CheckGLError();
}

void CLASS_NAME::OnDrawFrame() {
    GLApp::OnDrawFrame();

    // m_light_color = glm::vec3{glm::sin(glfwGetTime()), glm::cos(glfwGetTime()), 0};
    m_light_position.x = 7 * sin(glfwGetTime());
    m_light_position.z = 7 * (cos(glfwGetTime()));
    m_light_program.Use();
    m_light_program.SetUniform("lightColor", m_light_color);
    m_light_program.SetUniform("view", camera.GetViewMatrix());
    m_light_program.SetUniform("projection", GetProjectionMatrix());
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_light_position));
    model = glm::scale(model, glm::vec3(0.5f));
    m_cube_model->SetModelMatrix(model);
    m_cube_model->Draw(m_light_program);

    m_cube_program.Use();
    m_cube_program.SetUniform("view", camera.GetViewMatrix());
    m_cube_program.SetUniform("projection", GetProjectionMatrix());
    m_cube_program.SetUniform("material.shininess", 32.0f);
    m_cube_program.SetUniform("viewPos", camera.position);
    m_cube_program.SetUniform("light.position", m_light_position);
    m_cube_program.SetUniform("light.ambient", m_light_color * 0.2f);
    m_cube_program.SetUniform("light.diffuse", m_light_color * 0.5f);
    m_cube_program.SetUniform("light.specular", m_light_color * 0.3f);
    m_cube_program.SetUniform("light.kc", 1.0f);
    m_cube_program.SetUniform("light.kl", 0.039f);
    m_cube_program.SetUniform("light.kq", 0.032f);
    m_cube_program.SetUniform("flashLight.position", camera.position);
    m_cube_program.SetUniform("flashLight.direction", camera.front);
    m_cube_program.SetUniform("flashLight.cutOff", glm::cos(glm::radians(15.0f)));
    m_cube_program.SetUniform("flashLight.outerCutOff", glm::cos(glm::radians(16.0f)));
    m_cube_program.SetUniform("flashLight.enable", m_flash_light_enable);
    int i = 1;
    for (auto & cubePos : cubePositions) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePos * 2.0f);
        float angle = 20.0f * i++ * glfwGetTime();
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_cube_model->SetModelMatrix(model);
        m_cube_model->Draw(m_cube_program);
    }

    CheckGLError();
}

void CLASS_NAME::ProcessInput(GLFWwindow *win) {
    if (glfwGetKey(win, GLFW_KEY_0) == GLFW_PRESS) {
        m_light_position.w = 0;
    }
    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) {
        m_light_position.w = 1;
    }
    if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
        m_flash_light_enable = true;
    }
    if (glfwGetKey(win, GLFW_KEY_G) == GLFW_PRESS) {
        m_flash_light_enable = false;
    }
    GLApp::ProcessInput(win);
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
