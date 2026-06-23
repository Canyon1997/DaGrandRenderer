#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 surfaceNormal;
out mat3 normMatrix;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));
    surfaceNormal = normalMatrix * aNorm;
    normMatrix = normalMatrix;
}