#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 oPos;

uniform mat4x4 projMat;
uniform mat4x4 modelMat;

void main(){
	gl_Position = projMat * modelMat * vec4(aPos.xyz, 1.0);
	oPos = vec3(aPos.xyz);
}