#version 330

in vec4 vcolor;

uniform vec4 fcolor;

layout (location = 0) out vec4 out_color;

void main()
{
	out_color = fcolor * vcolor;
}