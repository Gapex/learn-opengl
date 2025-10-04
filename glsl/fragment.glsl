#version 410 core

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D boxTexture;
uniform sampler2D faceTexture;
uniform float timestamp;

out vec4 FragColor;

void main()
{
    // FragColor = vertexColor;
    float mixture = (sin(timestamp) + 1) / 2.0;
    FragColor = mix(texture(boxTexture, TexCoord), texture(faceTexture, TexCoord), mixture);
    // FragColor = texture(boxTexture, TexCoord);
    // FragColor = texture(faceTexture, TexCoord);
}