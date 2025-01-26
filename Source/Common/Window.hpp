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

	bool IsKeyDown(Input::Key key) const;

	bool IsKeyPressed(Input::Key key) const;

	bool IsKeyReleased(Input::Key key) const;

	void LockCursor();

	void UnlockCursor();

	glm::vec2 GetCursorMovement() const;

	glm::vec2 GetScrollMovement() const;

private:
	GLFWwindow* window;
};
