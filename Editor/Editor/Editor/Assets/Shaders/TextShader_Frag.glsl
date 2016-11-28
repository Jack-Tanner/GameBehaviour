#version 150 core
in vec2 Texcoord;

uniform sampler2D FontGlyph;
uniform vec4 FontColour;

out vec4 outColor;
void main()
{
    outColor = FontColour * vec4( 1.0f, 1.0f, 1.0f, texture( FontGlyph, Texcoord ) );
}