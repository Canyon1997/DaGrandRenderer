#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix; // view space

// in view space
out vec3 fragPos;
out vec2 texCoord;
out vec3 surfaceNormal;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    fragPos = vec3(view * model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    surfaceNormal = normalMatrix * aNorm;
}