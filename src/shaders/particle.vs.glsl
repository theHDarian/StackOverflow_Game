#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;

uniform mat4 projection;
uniform mat4 transform;

out vec4 color;

void main()
{
	color = in_color;
	gl_Position = projection * transform * vec4(in_position, 1.0);
}