#pragma once

#include <array>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include "Vec3D.hpp"

template <typename T> class Matrix4 {
public:
  static_assert(std::is_arithmetic<T>::value,
                "can not create non-arithmatic Matrix4");

  Matrix4() = default;
  Matrix4(const std::vector<std::vector<T>> &values) {
    for (size_t i = 0; i < std::min(values.size(), 4); ++i) {
      for (size_t j = 0; j < std::min(values[i].size(), 4); ++j) {
        matrix[i][j] = values[i][j];
      }
    }
  }

  Matrix4(const std::vector<T> &values) {
    size_t k = 0;
    for (size_t i = 0; i < 4 && k < values.size(); ++i) {
      for (size_t j = 0; j < 4 && k < values.size(); ++j) {
        matrix[i][j] = values[k];
      }
    }
  }

  Matrix4<T> &operator+=(T scalar) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        matrix[i][j] += scalar;
      }
    }
    return *this;
  }

  Matrix4<T> operator+(T scalar) const {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] + scalar;
      }
    }
    return *this;
  }

  Matrix4<T> &operator+=(T scalar) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        matrix[i][j] += scalar;
      }
    }
    return *this;
  }

  Matrix4<T> operator-(T scalar) const {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] - scalar;
      }
    }
    return *this;
  }
  Matrix4<T> &operator-=(T scalar) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        matrix[i][j] -= scalar;
      }
    }
    return *this;
  }

  Matrix4<T> operator*(T scalar) const {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] * scalar;
      }
    }
    return result;
  }
  Matrix4<T> &operator/=(T scalar) {
    if (IsZeroScalar(scalar)) {
      throw std::logic_error("Matrix4f dividing 0");
    }
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        matrix[i][j] *= scalar;
      }
    }
    return *this;
  }

  Matrix4<T> operator/(T scalar) const {
    static_assert(scalar != 0, "divide 0 error!");
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] / scalar;
      }
    }
    return *this;
  }

  Matrix4<T> operator*(const Matrix4<T> &rhs) {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result[i][j] = 0;
        for (size_t k = 0; k < 4; ++k) {
          result[i][j] += matrix[i][k] * rhs.matrix[k][j];
        }
      }
    }
    return result;
  }

  Vec3<T> operator*(const Vec3<T> &vec3) const {
    Vec3<T> row1Vec(m11, m12, m13);
    Vec3<T> row2Vec(m21, m22, m23);
    Vec3<T> row3Vec(m31, m32, m33);
    return Vec3<T>(row1Vec * vec3, row2Vec * vec3, row3Vec * vec3);
  }

  static Matrix4<T> Ones() {
    static Matrix4<T> ones({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    return ones;
  }

  static Matrix4<T> Zeros() {
    static Matrix4<T> zeros;
    return zeros;
  }

  static Matrix4<T> Scale(T xScale, T yScale, T zScale) {
    return Matrix4<T>{
        xScale, 0, 0, 0, 0, yScale, 0, 0, 0, 0, zScale, 0, 0, 0, 0, 1,
    };
  }

  static Matrix4<T> Translate(T xTrans, T yTrans, T zTrans) {
    return Matrix4<T>{
        1, 0, 0, xTrans, 0, 1, 0, yTrans, 0, 0, 1, zTrans, 0, 0, 0, 1,
    };
  }

  static Matrix4<T> Rotate(T xRotate, T yRotate, T zRotate) {
    Matrix4<T> rotateMat = Matrix4<T>::Ones();
    if (!IsZeroScalar(xRotate)) {
      T theta = xRotate;
      Matrix4<T> xRotateMat{{1, 0, 0, 0},
                            {0, std::cos(theta), -std::sin(theta), 0},
                            {0, std::sin(theta), std::cos(theta), 0},
                            {0, 0, 0, 1}};
      rotateMat = rotateMat * xRotateMat;
    }

    if (!IsZeroScalar(yRotate)) {
      T theta = yRotate;
      Matrix4<T> yRotateMat{{std::cos(theta), 0, std::sin(theta), 0},
                            {0, 1, 0, 0},
                            {-std::sin(theta), 0, std::cos(theta), 0},
                            {0, 0, 0, 1}};
      rotateMat = rotateMat * yRotateMat;
    }
    if (!IsZeroScalar(zRotate)) {
      T theta = zRotate;
      Matrix4<T> zRotateMat{{std::cos(theta), -std::sin(theta), 0, 0},
                            {std::sin(theta), std::cos(theta), 0, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 1}};
      rotateMat = rotateMat * zRotateMat;
    }
    return rotateMat;
  }

private:
  static bool IsZeroScalar(T scalar) {
    if (std::is_integral<T>::value && scalar == T()) {
      return true;
    }
    if (std::is_floating_point<T>::value &&
        std::fabs(scalar) < std::numeric_limits<double>::epsilon()) {
      return true;
    }

    return false;
  }

  union {
    std::array<std::array<T, 4>, 4> matrix;
    struct {
      union {
        std::array<T, 4> row0;
        struct {
          T m11;
          T m12;
          T m13;
          T m14;
        };
      };
      union {
        std::array<T, 4> row1;
        struct {
          T m21;
          T m22;
          T m23;
          T m24;
        };
      };
      union {
        std::array<T, 4> row2;
        struct {
          T m31;
          T m32;
          T m33;
          T m34;
        };
      };
      union {
        std::array<T, 4> row3;
        struct {
          T m41;
          T m42;
          T m43;
          T m44;
        };
      };
    };
  };
};