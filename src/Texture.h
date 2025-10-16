#pragma once

#include <string>
#include "glad/glad.h"

struct Texture {
    unsigned int id{};
    unsigned int type = GL_TEXTURE_2D;
    std::string uniformName{};
    std::string path{};
};
