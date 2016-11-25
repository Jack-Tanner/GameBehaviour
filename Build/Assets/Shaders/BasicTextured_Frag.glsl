#version 150 core
in vec2 Texcoord;

uniform sampler2D texColour;

out vec4 outColor;
void main()
{
    outColor = texture( texColour, Texcoord );
}