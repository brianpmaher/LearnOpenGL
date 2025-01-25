#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char* title);

	~Window();

	inline bool ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	inline void PollEvents()
	{
		glfwPollEvents();
	}

	inline void SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

private:
	GLFWwindow* window;
};
