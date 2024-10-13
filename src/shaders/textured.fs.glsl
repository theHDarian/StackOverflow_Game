#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int changeColor;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	// note: branches are expensive, consider using another shader instead?
	if (changeColor == 1){
	// okay this is very costly and slow
		color.r = (color.r > 0 ? fcolor.r :0 );
		color.g = (color.g > 0 ? fcolor.g :0 );
		color.b = (color.b > 0 ? fcolor.b :0 );
	}
}
