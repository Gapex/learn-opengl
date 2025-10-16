file(GLOB PROJECT_SRCS *.cpp)
add_library(gypgltool
        ${PROJECT_SRCS}
        ${GLAD_SRC}
        ${GLFW_SRCS}
)
#target_link_libraries(main GL glfw pthread X11 dl Xi)
target_link_libraries(gypgltool opengl32 glfw assimp::assimp)

if (APPLE)
    message(STATUS "Apple = true")
    target_link_libraries(main "-framework OpenGL")
    target_link_libraries(main "-framework GLUT")
endif ()