#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 

uniform mat4x4 Projection;
uniform mat4x4 View;
uniform mat4x4 Model;

out vec3 Color;

void main()
{
	mat4x4 PVM = Projection * View * Model;	
	gl_Position = PVM * vec4(aPos, 1.0);
	Color = aColor;
}
