#version 330

// From vertex shader
in vec2 texcoord;

layout(location = 0) out vec4 color;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;

// Output color

void main()
{
	color = vec4(fcolor, 1.0f) * texture(sampler0, texcoord);
}
