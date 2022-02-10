#version 450
uniform vec3 ambientColor;

uniform sampler2D ColorMap0;
uniform bool ColorMap0Enabled;

uniform sampler2D ColorMap1;
uniform bool ColorMap1Enabled;

uniform sampler2D ColorMap2;
uniform bool ColorMap2Enabled;

uniform sampler2D ColorMap3;
uniform bool ColorMap3Enabled;

in highp vec3 fragPos;
in highp vec2 textureCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 resultCol = ambientColor;

    fragColor = vec4(resultCol, 1.0);

    if (ColorMap0Enabled)
        fragColor *= texture(ColorMap0, textureCoord);
    if (ColorMap1Enabled)
        fragColor *= texture(ColorMap1, textureCoord);
    if (ColorMap2Enabled)
        fragColor *= texture(ColorMap2, textureCoord);
    if (ColorMap3Enabled)
        fragColor *= texture(ColorMap3, textureCoord);
}
