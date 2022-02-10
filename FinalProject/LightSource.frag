#version 450
uniform lowp vec3 lightColor;
uniform sampler2D ColorMap0;
uniform bool ColorMap0Enabled;

in vec2 textureCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(lightColor, 1.0);

    if (ColorMap0Enabled){
        fragColor *= texture(ColorMap0, textureCoord);
    }
}
