cmake_minimum_required(VERSION 3.21)
project(learn-opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES  "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)

set(OPEN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/open_source)

# set up GLFW
set(GLFW_DIR ${OPEN_SOURCE_DIR}/glfw)
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)
# end setup GLFW

# set up glad
set(GLAD_DIR ${OPEN_SOURCE_DIR}/glad)
include_directories(${GLAD_DIR}/include)
set(GLAD_SRC
        ${GLAD_DIR}/src/glad.c)
# set up glad end

# set up stb_image
set(STB_IMG_DIR ${OPEN_SOURCE_DIR}/stb-image)
include_directories(${STB_IMG_DIR})
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
# set up stb_image end
include_directories(/opt/homebrew/include)

# set up glm
set(GLM_DIR ${OPEN_SOURCE_DIR}/glm)
include_directories(${GLM_DIR})
# set up end glm

link_directories(/opt/homebrew/lib)

add_executable(main main.cpp
        ${GLAD_SRC}
        ${GLFW_SRCS}
        GLApp.cpp
        program.cpp
        shader.cpp
)
target_link_libraries(main GL glfw pthread X11 dl Xi)

if (APPLE)
    message(STATUS "Apple = true")
    target_link_libraries(main "-framework OpenGL")
    target_link_libraries(main "-framework GLUT")
endif ()
