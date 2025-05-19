// ref: https://learnopengl.com/In-Practice/Text-Rendering
// optimizations made based on: https://www.youtube.com/watch?v=S0PyZKX4lyI

#version 330 core

in vec2 vertex;

out VS_OUT {
    vec2 TexCoords;
    vec4 TexColor;
    flat int index; 
}vs_out;

// larger the array = more letters able to draw at once
// but each individual graphics card has a limit
// should consult limit online
uniform mat4 transforms[200]; // to transform each letter
uniform vec4 colors[200];
uniform mat4 projection;

void main()
{
    // index of array = instance #
    gl_Position = projection * transforms[gl_InstanceID] * vec4(vertex.xy, 0.0, 1.0);
    vs_out.index = gl_InstanceID;
    vs_out.TexCoords = vertex.xy;
    vs_out.TexColor = colors[gl_InstanceID];
}  