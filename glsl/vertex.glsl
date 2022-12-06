#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform float timestamp;
uniform mat4 trans;

void main()
{
    // gl_Position = trans * vec4(aPos.x + (sin(timestamp) / 2), aPos.y, aPos.z, 1.0);
    gl_Position = trans * vec4(aPos.xyz, 1.0);
    if (aPos.x > 0) {
        vertexColor = vec4(0.3, 0.6, 0.45, 1.0);
    } else {
        vertexColor = vec4(0.3, 0.3, 0.45, 1.0);
    }
    vertexColor = vec4(0.2, sin(timestamp) / 2.0 + 0.5, 0.3, 1.0);
    TexCoord = aTexCoord;
}