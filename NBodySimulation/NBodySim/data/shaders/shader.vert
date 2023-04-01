#version 430 core

layout (location = 0) in vec3 translation;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aColor; 
layout (location = 3) in vec2 aTexCoord;

uniform mat4x4 ProjView;

uniform mat4x4 Model;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	vec4 pos = Model * vec4(aPos, 1.0) + vec4(translation, 1.0);
	gl_Position = ProjView * pos;
	Color = aColor;
	TexCoord = aTexCoord;
}
