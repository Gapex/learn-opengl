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
#add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
# set up stb_image end
include_directories(/opt/homebrew/include)

# set up glm
set(GLM_DIR ${OPEN_SOURCE_DIR}/glm)
include_directories(${GLM_DIR})
# set up end glm

# set up imgui
set(IMGUI_DIR ${OPEN_SOURCE_DIR}/imgui)
include_directories(${IMGUI_DIR}/ ${IMGUI_DIR}/backends)
file(GLOB IMGUI_SRCS ${IMGUI_DIR}/*.cpp)
#message(FATAL_ERROR ${IMGUI_SRCS})
# end set up imgui

# set up assimp
set(ASSIMP_DIR ${OPEN_SOURCE_DIR}/assimp)
# 配置Assimp编译选项（可选）
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)  # 不编译工具
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)         # 不编译测试
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)             # 不执行安装
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
# 根据需要启用特定格式的导入/导出器
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)   # 启用OBJ格式导入
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)  # 启用glTF格式导入
add_subdirectory(${ASSIMP_DIR})
# end set up assimp

link_directories(/opt/homebrew/lib)

add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
add_subdirectory(src)
