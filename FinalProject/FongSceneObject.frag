#version 450
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLightSource
{
    lowp vec3 color;
    float intensity;
    highp vec3 direction;
};

struct PointLightSource
{
    lowp vec3 color;
    highp vec3 position;

    float intensity;
    float constFactor;
    float linFactor;
    float quadFactor;
};

struct SpotLightSource
{
    lowp vec3 color;
    highp vec3 position;
    highp vec3 direction;

    float cutOff;
    float outerCutOff;

    float intensity;
    float constFactor;
    float linFactor;
    float quadFactor;
};

uniform vec3 ambientColor;
uniform vec3 cameraPos;
uniform DirLightSource dirLights[10];
uniform int dirLightsCount;

uniform PointLightSource pointLights[10];
uniform int pointLightsCount;

uniform SpotLightSource spotLights[10];
uniform int spotLightsCount;

uniform Material material;

vec3 CalcDirLight(DirLightSource light, vec3 normal, vec3 toEye)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0), material.shininess) * sign(diff);
    vec3 diffuse  = diff * material.diffuse;
    vec3 specular = spec * material.specular;
    return light.intensity * light.color * (diffuse + specular);
}

vec3 CalcPointLight(PointLightSource light, vec3 normal, vec3 vertexPos, vec3 toEye)
{
    vec3 lightDir = normalize(light.position - vertexPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0), material.shininess) * sign(diff);
    float distance = length(light.position - vertexPos);
    float attenuation = 1.0 / (light.constFactor + light.linFactor * distance +
                             light.quadFactor * distance * distance);
    vec3 diffuse  = diff * material.diffuse;
    vec3 specular = spec * material.specular;

    return light.intensity * attenuation * (diffuse + specular) * light.color;
}

vec3 CalcSpotLight(SpotLightSource light, vec3 normal, vec3 vertexPos, vec3 toEye)
{
    vec3 lightDir = normalize(light.position - vertexPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0), material.shininess) * sign(diff);
    float distance = length(light.position - vertexPos);
    float attenuation = 1.0 / (light.constFactor + light.linFactor * distance +
                             light.quadFactor * distance * distance);
    float theta = acos(dot(lightDir, normalize(-light.direction)));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = light.intensity * clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 diffuse  = diff * material.diffuse;
    vec3 specular = spec * material.specular;

    return attenuation * intensity * (diffuse + specular) * light.color;
}

in highp vec3 fragPos;
in highp vec2 textureCoord;
in highp mat3 TBN;

layout(location = 0) out vec4 fragColor;

struct TextureMap{
    sampler2D map;
    bool enabled;
};

uniform TextureMap ColorMap[10];
uniform int ColorMapCount;

uniform sampler2D NormalMap0;
uniform bool NormalMap0Enabled;


vec3 CalcNormal()
{
    vec3 normal_rgb = texture(NormalMap0, textureCoord).rgb;
    normal_rgb = normalize(normal_rgb * 2.0 - 1.0);
    return normalize(TBN * normal_rgb);
}

void main()
{
    vec3 toEye = normalize(cameraPos - fragPos);
    vec3 resultCol = ambientColor;
    vec3 mod_normal = NormalMap0Enabled ? CalcNormal() : TBN[2];

    for (int i = 0; i < min(dirLightsCount, 10); ++i)
    {
        resultCol += CalcDirLight(dirLights[i], mod_normal, toEye);
    }
    for (int i = 0; i < min(pointLightsCount, 10); ++i)
    {
        resultCol += CalcPointLight(pointLights[i], mod_normal, fragPos, toEye);
    }
    for (int i = 0; i < min(spotLightsCount, 10); ++i)
    {
        resultCol += CalcSpotLight(spotLights[i], mod_normal, fragPos, toEye);
    }

    fragColor = vec4(resultCol, 1.0);

    for (int i = 0; i < min(ColorMapCount, 10); ++i){
        if (ColorMap[i].enabled){
            fragColor *= texture(ColorMap[i].map, textureCoord);
        }
    }
}
