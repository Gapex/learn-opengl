#pragma

#include <array>

template <typename T, size_t N = 3>
class VecN {
public:
    VecN() = default;
    VecN(const std::initializer_list<T>& vals)
        : mData(vals)
    {
    }

    VecN(T x, T y, T z) : mData{x, y, z} {
        
    }

    T& operator[](size_t index)
    {
        return mData.at(index);
    }

    size_t Dim() const {
        return 3;
    }

    union {
        std::array<T, N> mData;
        struct {
            T x;
            T y;
            T z;
        };
    };
};

using Vertex = VecN<float, 3>;