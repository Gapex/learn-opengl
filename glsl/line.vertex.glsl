#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 trans;

void main() {
    gl_Position = trans * vec4(aPos, 1.0f);
    vertexColor = aColor;
}
