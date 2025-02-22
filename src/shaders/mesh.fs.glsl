#version 330

// From Vertex Shader
in vec2 vpos; // Distance from local origin

// Application data
uniform sampler2DArray sampler0;
uniform vec3 fcolor;
uniform int light_up;
uniform float time;
uniform float angle;

// Output color
layout(location = 0) out vec4 color;

//	https://www.shadertoy.com/view/lsjGWD
//	by Pietro De Nicola
//
#define OCTAVES   		3		// 7
#define SCALE   		3		// 7
#define BIAS   			0.2		// 7
#define POWER   		1		// 7
#define SWITCH_TIME 	60.0		// seconds

float t = 0 / SWITCH_TIME;

float function = mod(t, 4.0);
bool  multiply_by_F1 = mod(t, 8.0) >= 4.0;
bool  inverse = mod(t, 16.0) >= 8.0;
float distance_type = mod(t / 16.0, 4.0);

vec2 rotate(vec2 v, float a) {
	return vec2( v.x * cos(a) - v.y * sin(a), v.x * sin(a) + v.y * cos(a) );
}

vec2 hash(vec2 p) {
	p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
	return fract(sin(p) * 43758.5453);
}

float voronoi(in vec2 x) {
	vec2 n = floor(x);
	vec2 f = fract(x);

	float F1 = 8.0;
	float F2 = 8.0;

	for (int j = -1; j <= 1; j++)
		for (int i = -1; i <= 1; i++) {
			vec2 g = vec2(i, j);
			vec2 o = hash(n + g);

			o = 0.5 + 0.41 * sin(time / 10.f + 6.2831 * o);
			vec2 r = g - f + o;

			float d = distance_type < 1.0 ? dot(r, r) :				// euclidean^2
				distance_type < 2.0 ? sqrt(dot(r, r)) :			// euclidean
				distance_type < 3.0 ? abs(r.x) + abs(r.y) :		// manhattan
				distance_type < 4.0 ? max(abs(r.x), abs(r.y)) :	// chebyshev
				0.0;

			if (d < F1) {
				F2 = F1;
				F1 = d;
			}
			else if (d < F2) {
				F2 = d;
			}
		}

	float c = function < 1.0 ? F1 :
		function < 2.0 ? F2 :
		function < 3.0 ? F2 - F1 :
		function < 4.0 ? (F1 + F2) / 2.0 :
		0.0;

	if (multiply_by_F1)	c *= F1;
	if (inverse)			c = 1.0 - c;

	return c;
}

float fbm(vec2 p) {
	float s = 0.0;
	float m = 0.0;
	float a = 0.5;

	for (int i = 0; i < OCTAVES; i++) {
		s += a * voronoi(p);
		m += a;
		a *= 0.5;
		p *= 2.0;
	}
	return s / m;
}

void main()
{
	if (texture(sampler0, vec3(vpos * vec2(0.6) + vec2(0.5), 0)).a < 0.5) {
		float a = POWER * fbm(SCALE * rotate(vpos, angle)) + BIAS;
		a = (a > 0.5) ? 0.8 : (a > 0.3) ? 0.5 : 0.3;
		color = vec4(0.0, 1.0, 1.0, a);
	}
	else {
		float a = POWER * fbm(SCALE * rotate(vpos, angle)) - BIAS;
		color = vec4(0.0, 1.0, 1.0, 0.6 + a);
	}
}