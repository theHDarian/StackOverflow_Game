#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 translateAfterClamp;

void main()
{
   vec4 pos = translateAfterClamp * projection * view * model * vec4(in_position, 1.0);
   gl_Position = pos;
   texcoord = in_texcoord;
}