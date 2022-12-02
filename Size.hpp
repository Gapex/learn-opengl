#pragma

template <typename T>
struct Size {
public:
    Size(T width = T(), T height = T())
        : w(width)
        , h(height)
    {
    }
    T w;
    T h;
};