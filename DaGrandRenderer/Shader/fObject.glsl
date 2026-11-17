#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {

    vec3 cameraViewPosition;
    vec3 spotDirection;

    vec3 globalDirection;

    vec3 pointViewPosition;
    vec3 pointDirection;

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

void calcPhongShading(vec3 lightDir, vec3 surfNorm, out vec3 outAmb, out vec3 outDiff, out vec3 outSpec);
vec3 calcSpotLightShading();
vec3 calcPointLightShading();
vec3 calcDirectionalLightShading();
float calcAttenuation(vec3 lightPos);
float calcIntensity(vec3 lightDir);

void main()
{

    vec3 result;
    result += calcSpotLightShading();
    result += calcDirectionalLightShading();

    FragColor = vec4(result, 1.0);
}

void calcPhongShading(vec3 lightDir, vec3 surfNorm, out vec3 outAmb, out vec3 outDiff, out vec3 outSpec)
{

    outAmb = vec3(texture(material.diffuse, texCoord)) * light.ambient;

    float diffuseStrength = max(dot(surfNorm, lightDir), 0.0);
    outDiff = diffuseStrength * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectLightDir = reflect(-lightDir, surfNorm);
    float shininess = pow(max(dot(viewDir, reflectLightDir), 0.0), material.shininess);
    outSpec = vec3(texture(material.specular, texCoord)) * shininess * light.specular;
}

vec3 calcSpotLightShading()
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 surfNorm = normalize(surfaceNormal);
    vec3 lightDir = normalize(light.cameraViewPosition - fragPos);
    calcPhongShading(lightDir, surfNorm, ambient, diffuse, specular);

    float intensity = calcIntensity(lightDir);
    float attenuation = calcAttenuation(light.cameraViewPosition);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 calcPointLightShading()
{
    vec3 ret;

    return ret;
}


vec3 calcDirectionalLightShading()
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 surfNorm = normalize(surfaceNormal);
    vec3 lightDir = normalize(-light.globalDirection);
    calcPhongShading(lightDir, surfNorm, ambient, diffuse, specular);

    return ambient + diffuse + specular;
}

float calcAttenuation(vec3 lightPos)
{
    float distance = length(light.cameraViewPosition - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return attenuation;
}

float calcIntensity(vec3 lightDir)
{
    float theta = dot(lightDir, normalize(-light.spotDirection));
    float epsilon = light.innerCutOff - light.outerCutOff;
    return clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
}