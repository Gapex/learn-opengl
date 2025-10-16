#include <map>

#include "ShowModelDemo.h"
#include "FileModel.h"

ShowModelDemo::ShowModelDemo(WindowInfo info) : GLApp(info) {}

void ShowModelDemo::Init() {
    GLApp::Init();
    bag_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "glsl/bag.vertex.glsl");
    bag_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/bag.frag.glsl");
    plane_program.AddShader(GL_VERTEX_SHADER, PROJECT_DIR "glsl/depth_test.vertex.glsl");
    plane_program.AddShader(GL_FRAGMENT_SHADER, PROJECT_DIR "glsl/depth_test.frag.glsl");
    if (!plane_program.Init()) {
        LOGE("Failed to initialize depth test program");
        return;
    }
    if (!bag_program.Init()) {
        LOGE("Failed to initialize bag program");
        return;
    }
    bagModel = std::make_unique<FileModel>(PROJECT_DIR "assets/backpack/backpack.obj");
    cubeModel = std::make_unique<FileModel>(PROJECT_DIR "assets/cube/cube.obj");
    Texture wallTexture;
    wallTexture.id = Model::TextureFromFile("marble.jpg", PROJECT_DIR "texture/");
    wallTexture.uniformName = "texture_diffuse0";
    cubeModel->meshes.front().AddTexture(wallTexture);

    planeModel = std::make_unique<Model>();
    Mesh &planeMesh = planeModel->meshes.emplace_back();
    std::vector<glm::vec3> planeVertices{{1, 0, 1}, {-1, 0, 1}, {-1, 0, -1}, {1, 0, 1}, {-1, 0, -1}, {1, 0, -1}};
    std::vector<glm::vec2> planeTexCoords{{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}};
    std::vector<glm::vec3> planeNormals{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
    planeMesh.AddBuffer<glm::vec3>(0, BufferType::VERTEX_3D, planeVertices);
    planeMesh.AddBuffer<glm::vec3>(1, BufferType::NORMAL, planeNormals);
    planeMesh.AddBuffer<glm::vec2>(2, BufferType::TEX_COORD, planeTexCoords);
    planeModel->SetModelMatrix(glm::translate(planeModel->GetModelMatrix(), glm::vec3(0, planeHeight, 0)));
    planeMesh.SetIndices({0, 1, 2, 3, 4, 5});
    Texture planeTexture;
    planeTexture.id = Model::TextureFromFile("metal.png", PROJECT_DIR "texture/");
    planeTexture.uniformName = "texture_diffuse0";
    planeMesh.AddTexture(planeTexture);
    planeMesh.Setup();
    CheckGLError();

    transparent_window = std::make_unique<Model>();
    Mesh &transparent_window_mesh = transparent_window->meshes.emplace_back();
    transparent_window_mesh.AddBuffer(0, BufferType::VERTEX_3D, planeVertices);
    transparent_window_mesh.AddBuffer(2, BufferType::TEX_COORD, planeTexCoords);
    transparent_window_mesh.SetIndices({0, 1, 2, 3, 4, 5});
    Texture transparent_window_texture;
    transparent_window_texture.id =
        Model::TextureFromFile("transparent_window.png", PROJECT_DIR "texture/", GL_CLAMP_TO_EDGE);
    transparent_window_texture.uniformName = "texture_diffuse0";
    transparent_window_mesh.AddTexture(transparent_window_texture);
    transparent_window_mesh.Setup();
}

void ShowModelDemo::OnDrawFrame() {
    GLApp::OnDrawFrame();
    const glm::mat4 projection = GetProjectionMatrix();

    bag_program.Use();
    bag_program.SetUniform("view", camera.GetViewMatrix());
    bag_program.SetUniform("projection", projection);
    if (bagModel) {
        bagModel->Draw(bag_program);
    }
    if (cubeModel) {
        for (size_t i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions.at(i) * 2.0f);
            cubeModel->SetModelMatrix(model);
            cubeModel->Draw(bag_program);
        }
    }

    plane_program.Use();
    plane_program.SetUniform("projection", projection);
    plane_program.SetUniform("view", camera.GetViewMatrix());
    if (planeModel) {
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(planeScale, 1.0f, planeScale));
        model = glm::translate(model, glm::vec3(0, planeHeight, 0));
        planeModel->SetModelMatrix(model);
        planeModel->Draw(plane_program);
    }
    if (transparent_window) {
        std::map<float, glm::mat4> sorted_transparent_windows;
        for (size_t i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            glm::vec3 window_pos = cubePositions[i] * 2.0f;
            window_pos.y += 0.01f;
            window_pos.z += 1.5f;
            window_pos.x += glm::sin(glfwGetTime() * (i + 1));
            model = glm::translate(model, window_pos);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            sorted_transparent_windows[glm::length(camera.position - window_pos)] = model;
        }
        for (auto iter = sorted_transparent_windows.rbegin(); iter != sorted_transparent_windows.rend(); iter++) {
            transparent_window->SetModelMatrix(iter->second);
            transparent_window->Draw(plane_program);
        }
    }
}
