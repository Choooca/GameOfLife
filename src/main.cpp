#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <GLM/mat4x4.hpp>
#include <algorithm>
#include "render.hpp"
#include <thread>
#include <chrono>

using namespace std;

const int line = 100;
bool isPause = true;
bool alreadyPress = false;
void ChangePause()
{
	isPause = !isPause;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !alreadyPress)
	{
		alreadyPress = true;
		ChangePause();
	}

	if (alreadyPress && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		alreadyPress = false;
}
int CountNeighbourCell(int grid[line][line], int x, int y)
{

	int retCount = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if ((i == 0 && j == 0) || x + i < 0 || x + i > line - 1 || y + j < 0 || y + j > line - 1)
				continue;
			if (grid[x + i][y + j] == 1)
				retCount++;
		}
	}

	return retCount;
}

int main()
{

	Render render;

	render.RenderInit();

	int grid[line][line] = {};
	int previousGrid[line][line] = {};

	float blockWidth = 800.0f / line;
	float blockHeight = 600.0f / line;

	grid[50][50] = 1;
	grid[51][50] = 1;
	grid[52][50] = 1;
	grid[53][50] = 1;
	grid[50][51] = 1;
	grid[54][51] = 1;
	grid[50][52] = 1;
	grid[51][53] = 1;
	grid[54][53] = 1;

	while (!glfwWindowShouldClose(render.GetWindow()))
	{
		glClearColor(.0f, .0f, .0f, .0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (size_t i = 0; i < line; i++)
		{
			render.RenderLine(-400, -300 + (600.0f / line) * i, 400, -300 + (600.0f / line) * i);
			render.RenderLine(-400 + (800.0f / line) * i, -300, -400 + (800.0f / line) * i, 300);
		}

		for (size_t x = 0; x < line; x++)
		{
			for (size_t y = 0; y < line; y++)
			{
				if (grid[x][y])
				{
					render.RenderQuad(-400 + (800.0f / line) * x + blockWidth / 2.f, -300 + (600.0f / line) * y + blockHeight / 2.f, blockWidth, blockHeight);
				}
			}
		}

		glfwSwapBuffers(render.GetWindow());
		glfwPollEvents();

		processInput(render.GetWindow());

		if (isPause)
		{

			if (glfwGetMouseButton(render.GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				double xPos, yPos;
				glfwGetCursorPos(render.GetWindow(), &xPos, &yPos);
				int xIndex = int(floor(xPos / blockWidth)) + 1;
				int yIndex = int(floor(yPos / blockHeight)) + 1;
				if (xIndex >= 0 && xIndex <= line - 1 && yIndex >= 0 && yIndex <= line - 1)
					grid[xIndex][-yIndex] = 1;
			}

			if (glfwGetMouseButton(render.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				double xPos, yPos;
				glfwGetCursorPos(render.GetWindow(), &xPos, &yPos);
				int xIndex = int(floor(xPos / blockWidth)) + 1;
				int yIndex = int(floor(yPos / blockHeight)) + 1;
				if (xIndex >= 0 && xIndex <= line - 1 && yIndex >= 0 && yIndex <= line - 1)
					grid[xIndex][-yIndex] = 0;
			}

			continue;
		}

		std::copy(&grid[0][0], &grid[0][0] + line * line, &previousGrid[0][0]);
		for (size_t x = 0; x < line; x++)
		{
			for (size_t y = 0; y < line; y++)
			{
				grid[x][y] = 0;
				int count = CountNeighbourCell(previousGrid, x, y);

				if (previousGrid[x][y] == 0)
				{
					if (count == 3)
						grid[x][y] = 1;
				}
				if (previousGrid[x][y] == 1)
				{
					if (count == 3 || count == 2)
						grid[x][y] = 1;
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	glfwTerminate();
	return 0;
}