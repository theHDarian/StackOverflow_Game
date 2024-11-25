#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int changeColor = 0;
uniform float alpha = 1.0;
uniform float effectAlpha = 1.0;
uniform float time;

uniform int shape; //0 = rect, 1 = tri, 2 = circ
uniform vec2 scale;
uniform bool laser = false;
uniform bool onDeath = false;
uniform int effectSize = 1;
uniform vec3 bcolor1;
uniform vec3 bcolor2;
uniform vec3 bcolor3;
uniform vec3 bcolor4;
uniform vec3 bcolor5;

#define PI 3.14159

// Output color
layout(location = 0) out  vec4 color;

vec3 rectangle() {
	switch (effectSize) {
	case 2:
		if (texcoord.x < 0.5) return bcolor1;
		return bcolor2;
		break;
	case 3:
		if (texcoord.x < 0.333) return bcolor1;
		if (texcoord.x < 0.666) return bcolor2;
		return bcolor3;
		break;
	case 4:
		if (texcoord.x < 0.25) return bcolor1;
		if (texcoord.x < 0.50) return bcolor2;
		if (texcoord.x < 0.75) return bcolor3;
		return bcolor4;
		break;
	case 5:
		if (texcoord.x < 0.2) return bcolor1;
		if (texcoord.x < 0.4) return bcolor2;
		if (texcoord.x < 0.6) return bcolor3;
		if (texcoord.x < 0.8) return bcolor4;
		return bcolor5;
		break;
	}
	return bcolor1;
}

vec3 tri() {
	float targetA = 2.67795;
	float range	  =	0.92729;
	float a = (atan(texcoord.y - 0.5, texcoord.x - 1.0));
	if (a < 0) a += 2 * PI;
	switch (effectSize) {
	case 2:
		if (a > targetA + range * (1.0 / 2.0)) return bcolor1;
		return bcolor2;
		break;
	case 3:
		if (a > targetA + range * (2.0 / 3.0)) return bcolor1;
		if (a > targetA + range * (1.0 / 3.0)) return bcolor2;
		return bcolor3;
		break;
	case 4:
		if (a > targetA + range * (3.0 / 4.0)) return bcolor1;
		if (a > targetA + range * (2.0 / 4.0)) return bcolor2;
		if (a > targetA + range * (1.0 / 4.0)) return bcolor3;
		return bcolor4;
		break;
	case 5:
		if (a > targetA + range * (4.0 / 5.0)) return bcolor1;
		if (a > targetA + range * (3.0 / 5.0)) return bcolor2;
		if (a > targetA + range * (2.0 / 5.0)) return bcolor3;
		if (a > targetA + range * (1.0 / 5.0)) return bcolor4;
		return bcolor5;
		break;
	}
	return bcolor1;
}

vec3 circle() {
	float a = (atan(texcoord.y-0.5, texcoord.x-0.5) + PI) / (2 * PI);
	switch (effectSize) {
	case 2:
		if (a < 0.5) return bcolor1;
		return bcolor2;
		break;
	case 3:
		if (a < 0.333) return bcolor1;
		if (a < 0.666) return bcolor2;
		return bcolor3;
		break;
	case 4:
		if (a < 0.25) return bcolor1;
		if (a < 0.50) return bcolor2;
		if (a < 0.75) return bcolor3;
		return bcolor4;
		break;
	case 5:
		if (a < 0.2) return bcolor1;
		if (a < 0.4) return bcolor2;
		if (a < 0.6) return bcolor3;
		if (a < 0.8) return bcolor4;
		return bcolor5;
		break;
	}
	return bcolor1;
}

float laserColoring() {
	if (texcoord.y <= 0.5) return pow(1.8 * texcoord.y, 4);
	return 0.5 * pow(pow(1.8 * (texcoord.y - 1), 2), 2);
}


void main()
{
	color = vec4(vec3(0.0), 1.0);
	switch (shape) {
	case 0:
		color = vec4(rectangle(), 1.0);
		if (onDeath && distance(texcoord * scale, vec2(0.5) * scale) < 0.1 * scale.x) color.rgb = vec3(sin(time), 0.0, 0.0);
		break;
	case 1:
		color = vec4(tri(), 1.0);
		if (onDeath && distance(texcoord * scale, vec2(0.5 * (scale.y / scale.x), 0.5) * scale) < 0.1 * scale.x) color.rgb = vec3(sin(time), 0.0, 0.0);
		break;
	case 2:
		color = vec4(circle(), 1.0);
		if (onDeath && distance(texcoord * scale, vec2(0.5) * scale) < 0.1 * scale.x) color.rgb = vec3(sin(time), 0.0, 0.0);
		break;
	}
	color.a *= texture(sampler0, texcoord).a;

	if (laser) {
		color += vec4(vec3(laserColoring()), 0.0);
	}
}
