#version 150 core
in vec3 position;
in vec2 uv;

out vec2 Texcoord;

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProjection; 

void main()
{
	Texcoord = uv;
	gl_Position = matProjection * matView * matWorld * vec4(position, 1.0);
}
