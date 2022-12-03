#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float timestamp;

void main()
{
    gl_Position = vec4(aPos.x + (sin(timestamp) / 2), aPos.y + cos(timestamp) / 2, aPos.z, 1.0);
    if (aPos.x > 0) {
        vertexColor = vec4(0.3, 0.6, 0.45, 1.0);
    } else {
        vertexColor = vec4(0.3, 0.3, 0.45, 1.0);
    }
    vertexColor = vec4(0.2, sin(timestamp) / 2.0 + 0.5, 0.3, 1.0);
}