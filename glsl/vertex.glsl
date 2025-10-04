#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform float timestamp;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    // gl_Position = trans * vec4(aPos.x + (sin(timestamp) / 2), aPos.y, aPos.z, 1.0);
    gl_Position = (projMat * viewMat * modelMat) * vec4(aPos.xyz, 1.0);
    vertexColor = vec4(aColor, 1.0f);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;
    FragPos = vec3(modelMat * vec4(aPos, 1.0f));
}