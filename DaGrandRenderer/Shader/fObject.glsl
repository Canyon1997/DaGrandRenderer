#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec2 texCoord;

// view space
in vec3 fragPos;
in vec3 surfaceNormal;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 surfNorm = normalize(surfaceNormal);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;
    ambient *= attenuation;

    vec3 lightDir = normalize(light.position - fragPos); // -light.direction: directional light example
    float diffuseStrength = max(dot(surfNorm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * vec3(texture(material.diffuse, texCoord)) * light.diffuse;
    diffuse *= attenuation;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectLightDir = reflect(-lightDir, surfNorm);
    float shininess = pow(max(dot(viewDir, reflectLightDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, texCoord)) * shininess * light.specular;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}