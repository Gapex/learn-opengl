#pragma once

#include <array>
#include <cmath>
#include <type_traits>

template <typename T> class Vec3 {
public:
  static_assert(std::is_arithmetic<T>::value,
                "can not instance non-arithmatic Vec");
  Vec3() = default;

  explicit Vec3(std::array<T, 3> &values) : values(values) {}

  Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

  size_t Dim() const { return 3; }

  Vec3<T> operator+(T scalar) {
    Vec3<T> result = *this;
    result.x += scalar;
    result.y += scalar;
    result.z += scalar;
    return result;
  }

  Vec3<T> operator-(T scalar) {
    Vec3<T> result = *this;
    result.x -= scalar;
    result.y -= scalar;
    result.z -= scalar;
    return result;
  }
  Vec3<T> operator*(T scalar) {
    Vec3<T> result = *this;
    result.x *= scalar;
    result.y *= scalar;
    result.z *= scalar;
    return result;
  }
  Vec3<T> operator/(T scalar) {
    Vec3<T> result = *this;
    result.x /= scalar;
    result.y /= scalar;
    result.z /= scalar;
    return result;
  }

  Vec3<T> &operator+=(T scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
  }

  Vec3<T> &operator-=(T scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
  }

  Vec3<T> &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vec3<T> &operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  Vec3<T> operator-() {
    Vec3<T> result;
    result.x = -x;
    result.y = -y;
    result.z = -z;
    return result;
  }

  Vec3<T> operator+(const Vec3<T> &rhs) {
    Vec3<T> result;
    result.x = x + rhs.x;
    result.y = y + rhs.y;
    result.z = z + rhs.z;
    return result;
  }

  Vec3<T> operator-(const Vec3<T> &rhs) {
    Vec3<T> result;
    result.x = x - rhs.x;
    result.y = y - rhs.y;
    result.z = z - rhs.z;
    return result;
  }

  T operator*(const Vec3<T> &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  Vec3<T> operator^(const Vec3<T> &rhs) const {
    return Vec3<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                   x * rhs.y - y * rhs.x);
  }

  Vec3<T> operator+=(const Vec3<T> &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Vec3<T> operator-=(const Vec3<T> &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  T Length2() const { return x * x + y * y + z * z; }
  T Length() const { return std::sqrt(x * x + y * y + z * z); }

  void Normalize() {}

  union {
    struct {
      std::array<T, 3> values;
    };
    struct {
      T x;
      T y;
      T z;
    };
    int a;
  };
};

using Vec3f = Vec3<float>;
using Vertex = Vec3<float>;
using Vec3d = Vec3<double>;