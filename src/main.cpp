#include "gl_app.h"
#include "data_types.h"

int main() {
    WindowInfo window_info;
    window_info.width = 1100;
    window_info.height = 1000;
    window_info.title = "Learning OpenGL";
    const auto app = std::make_unique<GLApp>(window_info);
    app->Init();
    app->Run();

    return 0;
}