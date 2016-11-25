#version 150 core
in vec3 position;
in vec4 colour;

out vec4 Colour;

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProjection; 

void main()
{
	Colour = colour;
	gl_Position = matProjection * matView * matWorld * vec4(position, 1.0);
}
