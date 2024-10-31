#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2DArray sampler0;
uniform vec3 fcolor;
uniform int changeColor = 0;
uniform float alpha = 1.0;
uniform float effectAlpha = 1.0;
uniform int frame = 1;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	color = vec4(fcolor, alpha) * texture(sampler0, vec3(texcoord.x, texcoord.y, frame));
	// note: branches are expensive, consider using another shader instead?
	if (changeColor == 1){
		color.r = fcolor.r * effectAlpha + color.r * (1.0 - effectAlpha);
		color.g = fcolor.g * effectAlpha + color.g * (1.0 - effectAlpha);
		color.b = fcolor.b * effectAlpha + color.b * (1.0 - effectAlpha);
	}
}

