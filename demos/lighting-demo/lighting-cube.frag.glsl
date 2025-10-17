#version 410 core

out vec4 frag_color;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main() {
    // 计算环境光照
    const float ambientStrength = 0.1f;
    vec3 ambient = lightColor * ambientStrength;
    // 计算漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // 计算镜面反射光照
    const float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * cubeColor;
    frag_color = vec4(result, 1.0);
}