#pragma once

#include <string>
#include "glad/glad.h"

struct Texture {
    unsigned int id{};
    unsigned int type = GL_TEXTURE_2D;
    std::string uniformName{};
    std::string path{};
    int unit{-1};

    Texture(const std::string &uniformName, unsigned int textureUnit) : uniformName(uniformName), unit(textureUnit) {}

    Texture &SetPath(const std::string &path) {
        this->path = path;
        return *this;
    }

    Texture &SetType(unsigned int newType) {
        type = newType;
        return *this;
    }

    Texture &SetTextureId(unsigned int newId) {
        id = newId;
        return *this;
    }
};
