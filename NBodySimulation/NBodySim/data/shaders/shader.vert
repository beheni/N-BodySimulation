#version 430 core

layout (location = 0) in vec3 a_Translation;
layout (location = 1) in vec3 a_Pos;
layout (location = 2) in vec3 a_Color; 
layout (location = 3) in vec2 a_TexCoord;


uniform mat4x4 u_ProjView;
uniform mat4x4 u_CameraRotation;
uniform mat4x4 u_Model;

out vec3 v_Color;
out vec2 v_TexCoord;

float size(vec3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void main()
{
	vec4 position = u_CameraRotation * vec4(a_Pos, 1.0) + u_Model * vec4(a_Translation, 1.0);
	gl_Position = u_ProjView * position;
	v_Color	= vec3(max(0.0, 1.5 - size(a_Translation) / 80.0),0.5,pow(min(1.0, 0.5 + size(a_Translation)/80.0), 2));
	v_TexCoord = a_TexCoord;
}
