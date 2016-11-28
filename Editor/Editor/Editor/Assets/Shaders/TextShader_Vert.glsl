#version 150 core
in vec3 position;
in vec2 uv;

out vec2 Texcoord;

uniform mat4 matProjection; 

void main()
{
	gl_Position = matProjection  * vec4(position, 1.0);
	Texcoord = uv;
}
