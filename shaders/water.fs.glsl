#version 330

uniform sampler2D screen_texture;
uniform float time;
uniform float darken_screen_factor;

in vec2 texcoord;

layout(location = 0) out vec4 color;

vec2 distort(vec2 uv) 
{
	float freq = 10;  
    float amp = 0.003; 

    float x = cos(uv.y * freq + time) * amp;
    float y = sin(uv.x * freq + time) * amp;

	//if distance from any edge of screen is < 0.1, then reduce intensity of distortion
	vec2 distortion = vec2(x,y);
	float minDist = min(min(distance(1,uv.x),distance(0,uv.x)),min(distance(1,uv.y),distance(0,uv.y)));
	if (uv.x > 0.9 || uv.x < 0.1|| uv.y > 0.9 || uv.y < 0.1) {
		distortion *= minDist * 10.0; //interpolate between 1 and 0
	}
	uv += distortion;

	return uv;
}

vec4 color_shift(vec4 in_color) 
{
	in_color += vec4(0.2, 0.2,0, 0); //shift towards yellow

	return in_color;
}

vec4 fade_color(vec4 in_color) 
{
	if (darken_screen_factor > 0)
		in_color -= darken_screen_factor * vec4(0.8, 0.8, 0.8, 0);
	return in_color;
}

void main()
{
	vec2 coord = distort(texcoord);

    vec4 in_color = texture(screen_texture, coord);
    color = color_shift(in_color);
    color = fade_color(color);
}