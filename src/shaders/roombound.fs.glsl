#version 330

// From vertex shader
in vec2 texcoord;

layout(location=0)out vec4 color;

// Application data
uniform sampler2DArray sampler0;
uniform int frame=0;// 0 close, 1 locked, 2 open
uniform bool tile=false;
uniform vec2 tiling=vec2(1.,1.);

// Output color

void main()
{
	if(tile){
		color=texture(sampler0,vec3(texcoord.x*tiling.x,texcoord.y*tiling.y,frame));
		if(texcoord.x<.1)color-=vec4(vec3(.7*(.1-texcoord.x)),0.);
		if(texcoord.x>.9)color-=vec4(vec3(.7*(texcoord.x-.9)),0.);
	}
	else{
		color=texture(sampler0,vec3(texcoord.x,texcoord.y,frame));
	}
	
}
