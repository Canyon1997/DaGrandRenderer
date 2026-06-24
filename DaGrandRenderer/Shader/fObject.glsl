#version 330 core

// in view space
in vec3 fragPos;
in vec3 surfaceNormal;

uniform vec3 lightSourcePos; // in view space
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(surfaceNormal);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightSourcePos - fragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectLightDir = reflect(-lightDir, norm);
    float shininess = pow(max(dot(viewDir, reflectLightDir), 0.0), 32);
    vec3 specular = specularStrength * shininess * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}