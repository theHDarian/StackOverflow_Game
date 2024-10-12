#version 330

layout (location = 0) in vec3 in_position;
in vec4 in_color;

out vec4 vcolor;

uniform mat4 projection;
uniform mat4 transform;

void main()
{
    vcolor = in_color;
	gl_Position = projection * transform * vec4(in_position, 1.0);
}