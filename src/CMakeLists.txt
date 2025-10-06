add_executable(main main.cpp
        ${GLAD_SRC}
        ${GLFW_SRCS}
        gl_app.cpp
        program.cpp
        shader.cpp
        vertex_buffer.cpp
        triangle_vertex_buffer.cpp
        triangle_vertex_buffer.h
        line_vertex_buffer.cpp
        line_vertex_buffer.h
        ${IMGUI_SRCS}
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        camera.cpp
        mesh.cpp
        model.cpp
)
#target_link_libraries(main GL glfw pthread X11 dl Xi)
target_link_libraries(main opengl32 glfw assimp::assimp)

if (APPLE)
    message(STATUS "Apple = true")
    target_link_libraries(main "-framework OpenGL")
    target_link_libraries(main "-framework GLUT")
endif ()