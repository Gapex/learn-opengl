//
// Created by gao on 2025/10/2.
//

#ifndef LEARN_OPENGL_LOG_H
#define LEARN_OPENGL_LOG_H

#include <cstdio>
#include <filesystem>

inline std::string GetFileName(const std::filesystem::path &path) {
    std::filesystem::path p(path);
    return p.filename().string();
}

#define LOGD(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        fprintf(stdout, "[D] [%s:%d] [%s]: " fmt "\n", GetFileName(__FILE__).c_str(), __LINE__, __func__,              \
                ##__VA_ARGS__);                                                                                        \
    } while (0)

#define LOGE(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        fprintf(stderr, "[E] [%s:%d] [%s]: " fmt "\n", GetFileName(__FILE__).c_str(), __LINE__, __func__,              \
                ##__VA_ARGS__);                                                                                        \
    } while (0)

inline void CheckGLError() {
    auto errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        LOGE("Failed to initialize OpenGL context: 0x%04x", errCode);
    }
}

#endif // LEARN_OPENGL_LOG_H