#version 330

// Input attributes
in vec3 in_color;
in vec3 in_position;

out vec3 vcolor;

// Application data
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vcolor = in_color;
	vec4 pos = projection * view * model * vec4(in_position, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}