#pragma once

#include "program.hpp"
#include "assimp/scene.h"
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Texture {
    unsigned int id{};
    unsigned int type = GL_TEXTURE_2D;
    std::string uniformName{};
    std::string path{};
};

enum class BufferType {
    VERTEX_2D,
    VERTEX_3D,
    NORMAL,
    TEX_COORD,
    COLOR_2D,
    COLOR_3D,
    INDICES,
    MAX,
};

template <typename T> class Buffer {
  public:
    Buffer() = default;
    Buffer(BufferType type, std::vector<T> bufferData);
    [[nodiscard]] size_t Size() const;
    [[nodiscard]] bool Check() const;
    [[nodiscard]] size_t Capacity() const;
    [[nodiscard]] const T *Data() const;
    [[nodiscard]] size_t ElementSize() const;
    bool Append(T x, T y, T z);
    bool Append(T x, T y);

  private:
    BufferType type;
    std::vector<T> data;
};

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

class Model {
  public:
    void Draw(Program &shader);
    virtual ~Model() = default;
    static unsigned int TextureFromFile(const std::string &filename, const std::string &directory,
                                        GLint repeatType = GL_REPEAT);
    void SetModelUniformName(const std::string &name);
    void SetModelMatrix(const glm::mat4 &newModelMat);
    [[nodiscard]] const glm::mat4 &GetModelMatrix() const;
    std::vector<Mesh> meshes;

  protected:
    std::string modelMatrixName = "model";
    glm::mat4 modelMatrix{1.0f};
};

class FileModel : public Model {
  public:
    explicit FileModel(const std::string &path);
    ~FileModel() override;

  private:
    std::string directory{};
    std::unordered_set<std::string> textures_loaded;
    void LoadFromFile(const std::string &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    void LoadMaterialTextures(Mesh &mesh, const aiMaterial *material, aiTextureType type, const std::string &typeName);
};

class MemoryModel : public Model {
  public:
    MemoryModel();
    void addMesh(Mesh &&mesh);
    ~MemoryModel() override;
};

template <typename T>
Buffer<T>::Buffer(BufferType type, std::vector<T> bufferData) : type(type), data(std::move(bufferData)) {}

template <typename T> size_t Buffer<T>::Capacity() const {
    return data.size() * sizeof(T);
}

template <typename T> size_t Buffer<T>::Size() const {
    return data.size() / ElementSize();
}

template <typename T> size_t Buffer<T>::ElementSize() const {
    size_t elementSize = 1;
    switch (type) {
    case BufferType::COLOR_2D:
    case BufferType::VERTEX_2D:
    case BufferType::TEX_COORD:
        elementSize = 2;
        break;
    case BufferType::VERTEX_3D:
    case BufferType::COLOR_3D:
    case BufferType::NORMAL:
        elementSize = 3;
        break;
    case BufferType::INDICES:
    default:
        elementSize = 1;
    }
    return elementSize;
}

template <typename T> bool Buffer<T>::Check() const {
    return data.size() % ElementSize() == 0;
}

template <typename T> const T *Buffer<T>::Data() const {
    return data.data();
}

template <typename T> bool Buffer<T>::Append(T x, T y, T z) {
    static std::unordered_set validTypes{BufferType::COLOR_3D, BufferType::VERTEX_3D, BufferType::INDICES,
                                         BufferType::NORMAL};
    if (validTypes.find(type) == validTypes.end()) {
        return false;
    }
    data.reserve(data.size() + 3);
    data.push_back(x);
    data.push_back(y);
    data.push_back(z);
    return true;
}

template <typename T> bool Buffer<T>::Append(T x, T y) {
    static std::unordered_set validTypes{BufferType::COLOR_2D, BufferType::TEX_COORD, BufferType::VERTEX_2D};
    if (validTypes.find(type) == validTypes.end()) {
        return false;
    }
    data.reserve(data.size() + 2);
    data.push_back(x);
    data.push_back(y);
    return true;
}

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
