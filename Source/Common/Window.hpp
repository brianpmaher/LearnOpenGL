#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char* title);

	~Window();

	inline bool ShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	inline void PollEvents() const
	{
		glfwPollEvents();
	}

	inline void SwapBuffers() const
	{
		glfwSwapBuffers(window);
	}

	inline float GetAspect() const
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return (float)width / (float)height;
	}

private:
	GLFWwindow* window;
};
