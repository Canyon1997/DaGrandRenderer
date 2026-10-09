#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec2 texCoord;

// view space
in vec3 fragPos;
in vec3 surfaceNormal;
uniform vec3 lightSourcePos;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 surfNorm = normalize(surfaceNormal);

    vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

    vec3 lightDir = normalize(-light.direction); // lightSourcePos - fragPos
    float diffuseStrength = max(dot(surfNorm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectLightDir = reflect(-lightDir, surfNorm);
    float shininess = pow(max(dot(viewDir, reflectLightDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, texCoord)) * shininess * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}