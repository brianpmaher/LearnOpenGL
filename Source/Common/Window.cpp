#include "Window.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <exception>
#include <iostream>

struct Window::Internal
{
	bool firstCursorPos = true;

	Input input;
};

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Window::Window(int width, int height, const char* title)
	: internal(new Internal())
{
	std::cout << "Creating window: (" << width << ", " << height << ", \"" << title << "\")" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowUserPointer(window, internal);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

Window::~Window()
{
	std::cout << "Closing window" << std::endl;

	glfwDestroyWindow(window);

	delete internal;

	glfwTerminate();
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::Close()
{
	glfwSetWindowShouldClose(window, true);
}

void Window::PollEvents()
{
	internal->input.Reset();

	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

float Window::GetAspect() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return (float)width / (float)height;
}

bool Window::IsKeyDown(Input::Key key) const
{
	switch (key)
	{
	case Input::Key::Escape:
		return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	case Input::Key::W:
		return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	case Input::Key::A:
		return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	case Input::Key::S:
		return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	case Input::Key::D:
		return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	default:
		return false;
	}
}

bool Window::IsKeyPressed(Input::Key key) const
{
	switch (key)
	{
	case Input::Key::Escape:
		return internal->input.keyEscape == Input::Status::Press;
	case Input::Key::W:
		return internal->input.keyW == Input::Status::Press;
	case Input::Key::A:
		return internal->input.keyA == Input::Status::Press;
	case Input::Key::S:
		return internal->input.keyS == Input::Status::Press;
	case Input::Key::D:
		return internal->input.keyD == Input::Status::Press;
	default:
		return false;
	}
}

bool Window::IsKeyReleased(Input::Key key) const
{
	switch (key)
	{
	case Input::Key::Escape:
		return internal->input.keyEscape == Input::Status::Release;
	case Input::Key::W:
		return internal->input.keyW == Input::Status::Release;
	case Input::Key::A:
		return internal->input.keyA == Input::Status::Release;
	case Input::Key::S:
		return internal->input.keyS == Input::Status::Release;
	case Input::Key::D:
		return internal->input.keyD == Input::Status::Release;
	default:
		return false;
	}
}

void Window::LockCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::UnlockCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::vec2 Window::GetCursorMovement() const
{
	return internal->input.cursorDelta;
}

glm::vec2 Window::GetScrollMovement() const
{
	return internal->input.scrollDelta;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window::Internal& internal = *static_cast<Window::Internal*>(glfwGetWindowUserPointer(window));

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		internal.input.keyEscape = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_W:
		internal.input.keyW = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_A:
		internal.input.keyA = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_S:
		internal.input.keyS = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_D:
		internal.input.keyD = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	}
}

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window::Internal& internal = *static_cast<Window::Internal*>(glfwGetWindowUserPointer(window));

	if (internal.firstCursorPos)
	{
		internal.input.cursorPosition = glm::vec2(xpos, ypos);
		internal.input.cursorDelta = glm::vec2(0.0f);
		internal.firstCursorPos = false;
	}
	else
	{
		auto prevPosition = internal.input.cursorPosition;
		internal.input.cursorPosition = glm::vec2(xpos, ypos);
		internal.input.cursorDelta = internal.input.cursorPosition - prevPosition;
	}
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window::Internal& internal = *static_cast<Window::Internal*>(glfwGetWindowUserPointer(window));

	internal.input.scrollDelta = {xOffset, yOffset};
}
