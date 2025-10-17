#include "MaterialDemo.h"
#include "FileModel.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

CLASS_NAME::CLASS_NAME(WindowInfo info) : GLApp(info) {}

CLASS_NAME::~CLASS_NAME() = default;

void CLASS_NAME::Init() {
    GLApp::Init();

    m_light_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/material-demo/light.vertex.glsl");
    m_light_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/material-demo/light.frag.glsl");
    if (!m_light_program.Init()) {
        return;
    }
    m_cube_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "demos/material-demo/lighting-cube.vertex.glsl");
    m_cube_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "demos/material-demo/lighting-cube.frag.glsl");
    if (!m_cube_program.Init()) {
        return;
    }
    m_cube_model = std::make_unique<FileModel>(PROJECT_DIR "assets/cube/cube.obj");
    CheckGLError();

    // 初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark(); // 使用暗色主题

    // 绑定GLFW和OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void CLASS_NAME::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_showImGuiWindow) {
        ImGui::Begin("material select", &m_showImGuiWindow);
        ImGui::Separator();
        int i = 0;
        for (auto &[name, _] : m_materials) {
            i += 1;
            ImGui::SetNextItemWidth(300);
            if (ImGui::RadioButton(name.c_str(), m_material_name == name)) {
                m_material_name = name;
            }
            if (i % 7 == 0) {
                ImGui::NewLine();
            } else {
                ImGui::SameLine();
            }
        }

        // ImGui::Separator();
        // ImGui::Text("当前材质参数");
        //
        // ImGui::Separator();
        // ImGui::Text("光照设置");
        // ImGui::ColorEdit3("光源颜色", &m_light_color[0]);
        // ImGui::SliderFloat3("光源位置", &m_light_position[0], -5.0f, 5.0f);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    model = glm::scale(model, glm::vec3(0.5f));
    m_cube_model->SetModelMatrix(model);
    m_cube_model->Draw(m_light_program);

    m_cube_program.Use();
    m_cube_program.SetUniform("view", camera.GetViewMatrix());
    m_cube_program.SetUniform("projection", GetProjectionMatrix());
    Material &material = m_materials[m_material_name];
    SET_UNIFORM(m_cube_program, material.ambient);
    SET_UNIFORM(m_cube_program, material.diffuse);
    SET_UNIFORM(m_cube_program, material.specular);
    SET_UNIFORM(m_cube_program, material.shininess);
    m_cube_program.SetUniform("viewPos", camera.position);
    m_cube_program.SetUniform("light.position", m_light_position);
    m_cube_program.SetUniform("light.ambient", m_light_color);
    m_cube_program.SetUniform("light.diffuse", m_light_color);
    m_cube_program.SetUniform("light.specular", m_light_color);
    m_cube_model->SetModelMatrix(glm::mat4(1.0f));
    m_cube_model->Draw(m_cube_program);

    CheckGLError();
    RenderImGui();
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
