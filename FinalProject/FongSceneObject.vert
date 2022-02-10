#version 450
uniform highp mat4 model;
uniform highp mat4 normModel;
uniform highp mat4 projView;
uniform highp vec3 cameraPos;

layout (location = 0) in highp vec3 localPosition;
layout (location = 1) in highp vec3 localNormal;
layout (location = 2) in highp vec2 textCoord;


out highp vec3 fragPos;
out highp vec2 textureCoord;
out highp mat3 TBN;

void main(void)
{
    vec3 t = cross(localNormal, vec3(1, 1, 1));
    vec3 b = cross(localNormal, t);
    vec3 n = normalize(mat3(normModel) * localNormal);

    t = normalize(mat3(normModel) * t);
    b = normalize(mat3(normModel) * b);

    TBN = mat3(t, b, n);

    vec4 worldVertexPos = model * vec4(localPosition, 1.f);
    fragPos = worldVertexPos.xyz;
    textureCoord = textCoord;

    gl_Position = projView * worldVertexPos;
}
