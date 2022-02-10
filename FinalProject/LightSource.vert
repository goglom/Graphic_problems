#version 450
uniform highp mat4 model;
uniform highp mat4 normModel;
uniform highp mat4 projView;

layout (location = 0) in highp vec3 localPosition;
layout (location = 1) in highp vec3 localNormal;
layout (location = 2) in highp vec2 textCoord;

out vec2 textureCoord;

void main()
{
    textureCoord = textCoord;
    gl_Position = projView * model * vec4(localPosition, 1.f);
}
