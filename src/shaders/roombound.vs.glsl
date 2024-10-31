#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 transform;
uniform mat3 projection;

void main()
{
    
   float angle;

   // Create a transform that will skew our texture coords
   mat3 trans = mat3(
      1.0       , 0.0       , 0.0,
      0.0,      cos(angle),        -sin(angle),
      0.0       , sin(angle),        -cos(angle)
   );
   texcoord = in_texcoord;
    //scare according to percentage 
    vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}