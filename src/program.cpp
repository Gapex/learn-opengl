#include "program.hpp"

bool Program::Init() {
    if (initialized) {
        LOGD("initialized shader: %d", program_id);
        return false;
    }
    initialized = true;
    program_id = glCreateProgram();
    for (std::shared_ptr<Shader> &shader : shaders) {
        if (!shader->IsInitialized()) {
            shader->Init();
        }
        if (!shader->Check()) {
            return false;
        }
        glAttachShader(program_id, shader->GetId());
    }
    glLinkProgram(program_id);
    if (Check()) {
        shaders.clear();
        return true;
    }
    return false;
}

bool Program::Check() const {
    bool result = true;
    GLint success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLsizei infoLen;
        char infoLog[1024]{0};
        glGetProgramInfoLog(program_id, 1024, &infoLen, infoLog);
        LOGD("%s", infoLog);
        result = false;
    } else {
        // LOGD("program check pass: id=%d", program_id);
    }
    return result;
}

void Program::Activate() const {
    if (Check()) {
        glUseProgram(program_id);
    }
}

void Program::Append(const std::shared_ptr<Shader> &shader) {
    shaders.emplace_back(shader);
}

void Program::SetInt(const char *name, int value) const {
    glUniform1i(GetUniformLocation(name), value);
}

void Program::SetMat4(const char *name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::SetFloat(const char *name, float value) const {
    glUniform1f(GetUniformLocation(name), value);
}

void Program::SetVec3(const char *name, glm::vec3 value) const {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

GLint Program::GetUniformLocation(const char *name) const {
    const auto location = glGetUniformLocation(program_id, name);
    if (location == -1) {
        LOGE("uniform location not found: %s", name);
        return -1;
    }
    return location;
}
