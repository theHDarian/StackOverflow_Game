#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float chargeBoundary = 1.0;
uniform vec4 unchargedColor;
uniform int isVertical = 1;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(fcolor, 1.0) * vec4(texture(sampler0, texcoord));
	// note: branches are expensive, consider using another shader instead?
	if (texcoord.y < 1.0-chargeBoundary && isVertical == 1){
		color.r *= fcolor.r;
		color.g *= fcolor.g;
		color.b *= fcolor.b;
		color *= unchargedColor;
	}
	if (texcoord.x > chargeBoundary && isVertical == 0) {
		color.r *= fcolor.r;
		color.g *= fcolor.g;
		color.b *= fcolor.b;
		color *= unchargedColor;
	}
}

