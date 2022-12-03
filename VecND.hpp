#pragma once

#include <array>

template <typename T, size_t N = 3> class VecND {
public:
  VecND() = default;
  VecND(const std::initializer_list<T> &vals) : mData(vals) {}

  VecND(T x, T y, T z) : mData{x, y, z} {}

  VecND(T x, T y, T z, T w) : mData{x, y, z, w} {}

  T &operator[](size_t index) { return mData.at(index); }

  size_t Dim() const { return 3; }

  union {
    std::array<T, N> mData;
    struct {
      T x;
      T y;
      T z;
    };
  };
};

using Vertex = VecND<float, 3>;
using Vec4f = VecND<float, 4>;