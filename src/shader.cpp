#include "shader.hpp"

Shader::Shader(GLenum shaderType, const std::string &src) : src(src), shader_id(0), shader_type(shaderType) {}

Shader::~Shader() {
    LOGD("shader delete: type=%d, id=%d", shader_type, shader_id);
    glDeleteShader(shader_id);
}

bool Shader::Init() {
    if (initialized) {
        LOGE("initialized shader: %d", shader_id);
        return false;
    }
    initialized = true;
    shader_id = glCreateShader(shader_type);
    if (!GetShaderSource()) {
        LOGE("failed to load shader: %s", src.c_str());
        return false;
    }
    const char *srcData = src.data();
    // LOGD("\n%s", srcData);
    glShaderSource(shader_id, 1, &srcData, nullptr);
    glCompileShader(shader_id);
    return Check();
}

bool Shader::Check() const {
    bool result = true;
    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        char infoLog[1024]{0};
        glGetShaderInfoLog(shader_id, 1024, nullptr, infoLog);
        LOGE("%s\n%s", src.c_str(), infoLog);
        result = false;
    } else {
        LOGD("shader check pass, type=0x%05x, id=%d", shader_type, shader_id);
    }
    return result;
}

Shader::operator unsigned int() const {
    return shader_id;
}

Shader::operator bool() const {
    return IsInitialized();
}

bool Shader::GetShaderSource() {
    static std::regex shaderSourceRegex(R"(#version.*void.*main)");
    std::smatch match;
    if (std::regex_search(src, match, shaderSourceRegex)) {
        return true;
    }
    std::ifstream file(src);
    if (!file.is_open()) {
        return false;
    }
    src.clear();
    std::string buffer;
    while (getline(file, buffer)) {
        if (!buffer.empty()) {
            src += buffer;
        }
        src.push_back('\n');
    }
    return true;
}
