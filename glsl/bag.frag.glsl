#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_ambient0;
uniform sampler2D texture_normal0;

void main()
{
    vec4 FragColor0 = texture(texture_normal0, TexCoords);
    vec4 FragColor1 = texture(texture_ambient0, TexCoords);
    vec4 FragColor2 = texture(texture_specular0, TexCoords);
    FragColor = texture(texture_diffuse0, TexCoords);
}