#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aTexCoord;

uniform mat4x4 ProjView;

uniform mat4x4 Model;
//uniform vec3 Coord;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	//vec3 pos = aPos + Coord;

	mat4x4 PVM = ProjView * Model;	
	gl_Position = PVM * vec4(aPos, 1.0);
	Color = aColor;
	TexCoord = aTexCoord;
}
