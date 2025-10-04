#version 410 core

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D boxTexture;
uniform sampler2D faceTexture;

uniform float timestamp;
uniform int colorMode = 0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    switch (colorMode) {
        case 0: // object color
            FragColor = vertexColor;
        break;

        case 1: // texture 1
            FragColor = texture(boxTexture, TexCoord);
            break;

        case 2: // texture 2
            FragColor = texture(faceTexture, TexCoord);
            break;

        case 3: // mix texture
            float mixture = (sin(timestamp) + 1) / 2.0;
            FragColor = mix(texture(boxTexture, TexCoord), texture(faceTexture, TexCoord), mixture);
            break;
        case 4: // color with lighting
            const float ambientStrength = 0.1f;
            vec3 ambient = ambientStrength * lightColor;

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            const float specularStrength = 0.75f;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;

            vec3 resultLight = ambient + diffuse + specular;
            FragColor = vec4(resultLight, 1.0f) * vertexColor;
            break;
    }
}