#include "data_types.h"

Mesh::Mesh(Mesh &&other) noexcept {
    buffers = std::move(other.buffers);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    isSetup = other.isSetup;
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    other.isSetup = false;
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
    buffers = std::move(other.buffers);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    isSetup = other.isSetup;
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    other.isSetup = false;
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
    return *this;
}

bool Mesh::SetIndices(std::vector<unsigned int> indices) {
    if (indices.size() % 3 != 0) {
        LOGE("Invalid indices size: %llu", indices.size());
    }
    this->indices = Buffer(BufferType::INDICES, std::move(indices));
    return true;
}

void Mesh::AddIndex(const glm::vec3 &index) {
    this->indices.Append(static_cast<unsigned int>(index.x), static_cast<unsigned int>(index.y),
                         static_cast<unsigned int>(index.z));
}

void Mesh::AddTexture(Texture texture) {
    textures.emplace_back(std::move(texture));
}

const std::vector<Texture> &Mesh::Textures() const {
    return textures;
}

Mesh::~Mesh() {
    if (isSetup) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
}

size_t Mesh::AllBuffersCapacity() const {
    size_t capacity = 0;
    for (auto &[_, buffer] : buffers) {
        capacity += buffer.Capacity();
    }
    return capacity;
}

void Mesh::Setup() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(AllBuffersCapacity()), nullptr, GL_STATIC_DRAW);
    GLsizeiptr offset = 0;
    for (auto &[location, buffer] : buffers) {
        const auto capacity = static_cast<GLsizeiptr>(buffer.Capacity());
        const auto elementSize = static_cast<GLint>(buffer.ElementSize());
        glBufferSubData(GL_ARRAY_BUFFER, offset, capacity, buffer.Data());
        glVertexAttribPointer(location, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(float), (void *)(offset));
        glEnableVertexAttribArray(location);
        offset += capacity;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.Capacity()), indices.Data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    isSetup = true;
}

void Mesh::Draw(const Program &shader) const {
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textures[i].type, textures[i].id);
        shader.SetInt(textures[i].uniformName.c_str(), i);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.Size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
