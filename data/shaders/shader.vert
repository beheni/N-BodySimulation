#version 430 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Color; 
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_ID;

layout(std430, binding = 1) buffer positionsBuffer
{
    vec4 positionsSSBO[];
};

uniform mat4x4 u_ProjView;
uniform mat4x4 u_Model;
uniform mat4x4 u_CameraRotation;
//uniform vec3   u_CameraPosition;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
	vec4 translation = positionsSSBO[int(a_ID)];
	vec4 starLocation = u_Model * translation;

	//float scale = length(starLocation.xyz - u_CameraPosition) / 400.0f;
	vec4 position = u_CameraRotation * vec4(a_Pos, 0.2) + starLocation;
	//vec4 position = u_CameraRotation * vec4(a_Pos * scale, 0.2) + starLocation;

	gl_Position = u_ProjView * position;
	v_Color	= a_Color;
	v_TexCoord = a_TexCoord;
}
