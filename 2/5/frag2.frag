#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    // vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    sampler2D emission;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main() {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diff
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(spec * texture(material.specular, TexCoords));

    // vec3 result = ambient + diffuse + specular + texture(material.emission, TexCoords).rgb * 0.5;
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    if (theta < light.outerCutOff) {
        diffuse = vec3(0.0);
        specular = vec3(0.0);
    } else if (theta < light.cutOff) {
        float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
    }

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}