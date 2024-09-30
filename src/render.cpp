#include "render.hpp"

using namespace std;

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Render::RenderInit()
{
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return exit(-1);
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(800, 600, "GameOfLife", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to open GLFW window" << endl;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return exit(-1);
	}

	glViewport(0, 0, 800, 600);

	Render::shader = new Shader("C:/LearnCpp/GameOfLife/shaders/default.vert", "C:/LearnCpp/GameOfLife/shaders/default.frag");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	projMat = CreateProjMat(0, 0, 800, 600);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderQuadInit();
	RenderLineInit();
}

GLFWwindow *Render::GetWindow()
{
	return window;
}

void Render::RenderQuadInit()
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3};

	glGenVertexArrays(1, &(Render::VAOquad));
	glGenBuffers(1, &(Render::VBOquad));
	glGenBuffers(1, &(Render::EBOquad));

	glBindVertexArray(Render::VAOquad);

	glBindBuffer(GL_ARRAY_BUFFER, Render::VBOquad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBOquad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::RenderLineInit()
{
	glGenVertexArrays(1, &(Render::VAOLine));
	glGenBuffers(1, &(Render::VBOLine));

	glBindVertexArray(Render::VAOLine);

	glBindBuffer(GL_ARRAY_BUFFER, Render::VBOLine);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::RenderLine(float startX, float startY, float endX, float endY)
{
	Render::shader->use();
	glLineWidth(.5);

	float x = endX - startX;
	float y = endY - startY;
	float line[6] = {
		0, 0, 0,
		x, y, 0};

	glm::mat4x4 translateMat = glm::translate(glm::mat4x4(1.), glm::vec3(startX, startY, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(Render::shader->ID, "modelMat"), 1, GL_FALSE, &translateMat[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Render::shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);
	shader->setFloat("ourAlpha", 0.1f);

	glBindVertexArray(VAOLine);

	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line), line);

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void Render::RenderQuad(float posX, float posY, float sizeX, float sizeY)
{
	Render::shader->use();
	glBindVertexArray(VAOquad);

	glm::mat4x4 model = glm::mat4x4(1.f);
	glm::mat4x4 sizeMat = glm::scale(glm::mat4x4(1.), glm::vec3(sizeX, sizeY, 1.f));
	glm::mat4x4 translateMat = glm::translate(glm::mat4x4(1.), glm::vec3(posX, posY, 0.0f));
	model = translateMat * sizeMat;

	shader->setFloat("ourAlpha", 1.0f);

	glUniformMatrix4fv(glGetUniformLocation(Render::shader->ID, "modelMat"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Render::shader->ID, "projMat"), 1, GL_FALSE, &projMat[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::mat4x4 Render::CreateProjMat(float xPos, float yPos, float xSize, float ySize)
{
	// l: xmin t: xmax
	// b: ymin r: ymax
	glm::mat4x4 retMat(1.f);

	float l = xPos - xSize * .5f;
	float t = xPos + xSize * .5f;
	float b = yPos - ySize * .5f;
	float r = yPos + ySize * .5f;

	retMat = glm::ortho(l, t, b, r);

	return retMat;
}