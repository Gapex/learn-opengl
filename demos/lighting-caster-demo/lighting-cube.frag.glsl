#version 410 core

in vec2 TexCoord;
out vec4 frag_color;

uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec4 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float kc;
    float kl;
    float kq;
};

uniform Light light;

struct FlashLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    bool enable;
};

uniform FlashLight flashLight;

vec3 calculatLightColor(vec4 lightPos, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular,
float lightKc, float lightKl, float lightKq) {
    vec3 ambient = lightAmbient * texture(material.diffuse, TexCoord).xyz;
    // 计算漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    if (lightPos.w == 1.0f) {
        lightDir = normalize(lightPos.xyz);
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * texture(material.diffuse, TexCoord).xyz);
    // 计算镜面反射光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightSpecular * (spec * texture(material.specular, TexCoord).xyz);

    vec3 result = ambient + diffuse + specular;
    if (lightPos.w == 0) {
        float distance = length(lightPos.xyz - FragPos);
        float attenuation = 1.0 / (lightKc + lightKl * distance + lightKq * pow(distance, 2.0f));
        result = attenuation * result;
    }
    return result;
}

void main() {
    vec3 result = vec3(0, 0, 0);
    result += calculatLightColor(light.position, light.ambient, light.diffuse, light.specular, light.kc, light.kl, light.kq);
    if (flashLight.enable) {
        vec3 flashLightDir = normalize(flashLight.position - FragPos);
        float theta = dot(flashLightDir, normalize(-flashLight.direction));
        float cutOffRange = flashLight.cutOff - flashLight.outerCutOff;
        float intensity = clamp((theta - flashLight.cutOff) / cutOffRange, 0.0, 1.0);
        if (theta > flashLight.cutOff) {
            intensity = 1.0;
        } else {
            intensity = clamp((theta - flashLight.outerCutOff) / (flashLight.cutOff - flashLight.outerCutOff), 0, 1);
        }
        vec3 flashLightResult = calculatLightColor(vec4(flashLight.position, 1), light.ambient * intensity, light.diffuse * intensity, light.specular * intensity, light.kc, light.kl, light.kq);
        result += flashLightResult;
    }
    frag_color = vec4(result, 1.0);
}