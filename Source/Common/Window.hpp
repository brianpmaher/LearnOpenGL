#pragma once

#include "Input.hpp"

#include <GLFW/glfw3.h>

class Window
{
public:
	struct Internal;

	Internal* internal;

	Window(int width, int height, const char* title);

	~Window();

	bool ShouldClose() const;

	void Close();

	void PollEvents();

	void SwapBuffers();

	float GetAspect() const;

	bool IsKeyDown(Input::Key key);

	bool IsKeyPressed(Input::Key key);

	bool IsKeyReleased(Input::Key key);

private:
	GLFWwindow* window;
};
