#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in float in_texIndex;

uniform mat4 projection;
uniform mat4 transform;

out vec4 color;
out vec2 texcoord;
out float texIndex;

void main()
{
	color = in_color;
	texcoord = in_texcoord;
	texIndex = in_texIndex;
	gl_Position = projection * transform * vec4(in_position, 1.0);
}