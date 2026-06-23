#version 330 core

in vec3 fragPos;
in vec3 surfaceNormal;
in mat3 normMatrix;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightSourcePos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(surfaceNormal);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightSourcePos - fragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;



    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}