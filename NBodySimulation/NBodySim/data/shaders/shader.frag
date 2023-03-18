#version 430 core

in vec3 Color;
out vec4 FragColor;

uniform float ColorScale;

void main()
{
	FragColor = vec4(Color * ColorScale, 1.0);
}
