#version 450
in lowp vec4 vertexColor;

void main()
{
    gl_FragColor = vertexColor;
}
