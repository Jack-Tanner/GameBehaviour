#version 150 core
in vec4 Colour;

uniform sampler2D texColour;

out vec4 outColor;
void main()
{
    outColor = Colour;
}