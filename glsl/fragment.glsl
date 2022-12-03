#version 330 core

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float timestamp;

out vec4 FragColor;

void main()
{
    // FragColor = vertexColor;
    float mixture = (cos(timestamp) + 1) / 2.0;
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), mixture);
}