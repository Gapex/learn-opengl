#pragma once

#include <array>
#include <cstdlib>

template <typename T>
struct Color {
public:
    Color(T red = T(), T green = T(), T blue = T(), T alpha = T())
        : mRGBA { red, green, blue, alpha }
    {
    }
    T& operator[](size_t index)
    {
        return mRGBA[index];
    }

    T& r() { return (*this)[0]; }
    T& g() { return (*this)[1]; }
    T& b() { return (*this)[2]; }
    T& a() { return (*this)[3]; }

private:
    std::array<T, 4> mRGBA;
};

using Color4f = Color<float>;
using Color4d = Color<double>;