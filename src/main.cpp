#include "gl_app.h"

int main() {
    WindowInfo window_info;
    window_info.width = 1100;
    window_info.height = 1000;
    window_info.title = "Learning OpenGL";
    GLApp app(window_info);
    app.run();
    return 0;
}