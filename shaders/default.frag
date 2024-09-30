#version 330 core
out vec4 FragColor;
in vec3 oPos;

uniform float ourAlpha = 1.0f;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, ourAlpha);
}