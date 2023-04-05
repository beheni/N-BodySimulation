#version 430 core

layout (location = 0) in int  a_ID;
layout (location = 1) in vec3 a_Pos;
layout (location = 2) in vec3 a_Color; 
layout (location = 3) in vec2 a_TexCoord;

uniform mat4x4 u_ProjView;
uniform mat4x4 u_Model;
uniform mat4x4 u_CameraRotation;
uniform sampler2D u_TexturePos;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
	vec3 translation = vec3(texture(u_TexturePos, vec2( float(a_ID/512), float(a_ID%512 ))));
	vec4 position = u_CameraRotation * vec4(a_Pos, 1.0) + vec4(translation, 0.0);
	gl_Position = u_ProjView * position;
	v_Color	= vec3(1.0);
	v_TexCoord = a_TexCoord;
}
