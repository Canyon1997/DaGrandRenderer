#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

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


float calcAttenuation();

void main()
{
    vec3 result;
    vec3 surfNorm = normalize(surfaceNormal);
    vec3 lightDir = normalize(light.position - fragPos); // -light.direction: directional light example

    // spot light calculations
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float attenuation = calcAttenuation();

    vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

    float diffuseStrength = max(dot(surfNorm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectLightDir = reflect(-lightDir, surfNorm);
    float shininess = pow(max(dot(viewDir, reflectLightDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, texCoord)) * shininess * light.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}

float calcAttenuation()
{
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return attenuation;
}