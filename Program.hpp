#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>

#include "Shader.hpp"

template <typename T>
using SP = std::shared_ptr<T>;

class Program {
public:
    Program() = default;

    bool Init() {
        if (initialized) {
            log_d("%s:%s: %s: %u", __FILE_NAME__, __LINE__, "initialized shader", programId);
            return false;
        }
        initialized = true;
        programId = glCreateProgram();
        for(SP<Shader> &shader : shaders) {
            if (!shader->IsInitialized()) {
                shader->Init();
            }
            if (!shader->Check()) {
                return false;
            }
            glAttachShader(programId, *shader);
        }
        glLinkProgram(programId);
        if(Check()) {
            shaders.clear();
            return true;
        }
        return false;
    }

    bool Check() const {
        bool result = true;
        GLint success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            GLsizei infoLen;
            char infoLog[1024];
            glGetProgramInfoLog(programId, 1024, &infoLen, infoLog);
            log_d("%s:%s: %s", __FILE_NAME__, __LINE__, infoLog);
            result = false;
        }
        return result;
    }

    void Activate() const {
        if (Check()) {
            glUseProgram(programId);
        }
    }

    void Append(const SP<Shader> &shader) {
        shaders.emplace_back(shader);
    }

private:
    std::vector<SP<Shader>> shaders;
    GLuint programId;
    bool initialized = false;
};