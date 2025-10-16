#pragma once

#include <vector>
#include <unordered_map>

#include "Texture.h"
#include "glm/glm.hpp"
#include "BufferType.h"
#include "Buffer.h"
#include "Program.h"

class Mesh {
  public:
    Mesh() = default;
    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;
    bool SetIndices(std::vector<unsigned int> indices);
    void AddIndex(const glm::vec3 &index);
    void AddTexture(Texture texture);
    const std::vector<Texture> &Textures() const;
    template <typename T> void AddBuffer(unsigned int location, BufferType type, std::vector<T> data);
    template <typename T> void AddBuffer(BufferType type, std::vector<T> data);

    void Draw(const Program &shader) const;
    void Setup();
    [[nodiscard]] bool IsSetup() const { return isSetup; }

    ~Mesh();

  private:
    size_t AllBuffersCapacity() const;
    std::unordered_map<unsigned int, Buffer<float>> buffers;
    Buffer<unsigned int> indices{BufferType::INDICES, {}};
    std::vector<Texture> textures{};
    bool isSetup{false};
    unsigned int vao{}, vbo{}, ebo{};
};

template <typename T> void Mesh::AddBuffer(unsigned int location, BufferType type, std::vector<T> data) {
    buffers[location] = Buffer(type, std::move(data));
}

template <> inline void Mesh::AddBuffer(unsigned int location, BufferType type, std::vector<glm::vec3> data) {
    std::vector<float> floatData(glm::value_ptr(data[0]), glm::value_ptr(data[0]) + data.size() * 3);
    buffers[location] = Buffer(type, std::move(floatData));
}
template <> inline void Mesh::AddBuffer(unsigned int location, BufferType type, std::vector<glm::vec2> data) {
    std::vector<float> floatData(glm::value_ptr(data[0]), glm::value_ptr(data[0]) + data.size() * 2);
    buffers[location] = Buffer(type, std::move(floatData));
}

template <typename T> void Mesh::AddBuffer(BufferType type, std::vector<T> data) {
    const size_t location = buffers.size();
    buffers[location] = Buffer(type, std::move(data));
}
