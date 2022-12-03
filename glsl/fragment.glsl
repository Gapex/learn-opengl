#version 330 core

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

out vec4 FragColor;

void main()
{
    // FragColor = vertexColor;
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}