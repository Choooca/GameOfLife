#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Render
{
public:
	void RenderInit();
	void RenderQuadInit();
	void RenderLineInit();
	void RenderQuad(float posX, float posY, float sizeX, float sizeY);
	void RenderLine(float startX, float startY, float endX, float endY);
	GLFWwindow *GetWindow();
	float ScreenWidth;
	float ScreenHeight;

private:
	GLFWwindow *window;
	Shader *shader;
	glm::mat4x4 projMat;
	unsigned int VAOquad, VBOquad, EBOquad;
	unsigned int VAOLine, VBOLine;
	glm::mat4x4 CreateProjMat(float xPos, float yPos, float xSize, float ySize);
};