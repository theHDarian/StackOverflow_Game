#version 330

// From vertex shader
in vec2 texcoord;

layout(location = 0) out vec4 color;

// Application data
uniform sampler2DArray sampler0;
uniform vec3 fcolor;
uniform int frame = 0; // 0 close, 1 locked, 2 open
uniform bool tile = false;
uniform vec2 tiling = vec2(1.0, 1.0);

// Output color

void main()
{
	if (tile) {
		color = vec4(fcolor, 1.0f) * texture(sampler0, vec3(texcoord.x * tiling.x, texcoord.y * tiling.y, 0));
		if (texcoord.x < 0.1) color -= vec4(vec3(0.7 * (0.1 - texcoord.x)), 0.0);
		if (texcoord.x > 0.9) color -= vec4(vec3(0.7 * (texcoord.x - 0.9)), 0.0);
	}
	else {
		color = vec4(fcolor, 1.0f) * texture(sampler0, vec3(texcoord.x, texcoord.y, frame));
	}
	
}
