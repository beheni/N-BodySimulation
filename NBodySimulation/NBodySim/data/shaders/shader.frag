#version 430 core

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

uniform float ColorScale;
uniform sampler2D ourTexture;

void main()
{
	vec4 col = texture(ourTexture, TexCoord);
	if (col.a < 0.15) discard;
	FragColor = col;
}
