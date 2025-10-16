#pragma once

#include <vector>
#include <unordered_set>

#include "BufferType.h"

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
