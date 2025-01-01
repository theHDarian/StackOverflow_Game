// ref: https://learnopengl.com/In-Practice/Text-Rendering
// optimizations made based on: https://www.youtube.com/watch?v=S0PyZKX4lyI

#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
layout (location = 0) in vec2 vertex; // since we now only use vec2 as vertex positions

out VS_OUT {
    vec2 TexCoords;
    vec4 TexColor;
    flat int index; 
}vs_out;
// Also pass along index so fs knows which char we're drawing. Q: what is flat??
// not a big thing but it's standard to group things together (why?)

// larger the array = more letters able to draw at once
// but each individual graphics card has a limit
// should consult limit online
uniform mat4 transforms[100]; // to transform each letter
uniform vec4 colors[100];
uniform mat4 projection;

void main()
{
    // index of array = instance #
    // just gotta make sure we line them up 
    gl_Position = projection * transforms[gl_InstanceID] * vec4(vertex.xy, 0.0, 1.0);
    vs_out.index = gl_InstanceID;
    vs_out.TexCoords = vertex.xy;
    vs_out.TexCoords.y = 1.0f - vs_out.TexCoords.y; 
    vs_out.TexColor = colors[gl_InstanceID];
    // note: our triangle strip is constructed upside down, so have to flip y (lol)
}  