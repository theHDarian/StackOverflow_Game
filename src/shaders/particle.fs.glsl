#version 330

in vec4 color;
in vec2 texcoord;
in float texIndex;
layout (location = 0) out vec4 out_color;

uniform sampler2D textures[2];

void main()
{
	int index = int(texIndex);
	out_color = vec4(texIndex,texIndex,texIndex,1.0);
}