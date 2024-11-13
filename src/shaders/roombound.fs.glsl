#version 330

// From vertex shader
in vec2 texcoord;

layout(location = 0) out vec4 color;

// Application data
uniform sampler2DArray sampler0;
uniform vec3 fcolor;
uniform int frame = 0; // 0 close, 1 locked, 2 open

// Output color

void main()
{
	color = vec4(fcolor, 1.0f) * texture(sampler0, vec3(texcoord.x, texcoord.y, frame));
}
