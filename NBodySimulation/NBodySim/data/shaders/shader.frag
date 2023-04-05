#version 430 core

in vec3 v_Color;
in vec2 v_TexCoord;
out vec4 v_FragColor;

uniform float u_ColorScale;
uniform sampler2D u_Texture;

void main()
{
	vec4 color = texture(u_Texture, v_TexCoord);
	//if (color.a < 0.15) discard;
	v_FragColor = color * vec4(v_Color, 1.0);
}
