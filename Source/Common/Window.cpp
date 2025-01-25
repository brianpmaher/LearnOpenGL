#include "Window.hpp"

#include <glad/glad.h>

#include <exception>
#include <iostream>

struct Window::Internal
{
	Input input;
};

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

Window::Window(int width, int height, const char* title) : internal(new Internal())
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
	internal->input = Input();

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

bool Window::IsKeyDown(Input::Key key)
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

bool Window::IsKeyPressed(Input::Key key)
{
	switch (key)
	{
	case Input::Key::Escape:
		return internal->input.KeyEscape == Input::Status::Press;
	case Input::Key::W:
		return internal->input.KeyW == Input::Status::Press;
	case Input::Key::A:
		return internal->input.KeyA == Input::Status::Press;
	case Input::Key::S:
		return internal->input.KeyS == Input::Status::Press;
	case Input::Key::D:
		return internal->input.KeyD == Input::Status::Press;
	default:
		return false;
	}
}

bool Window::IsKeyReleased(Input::Key key)
{
	switch (key)
	{
	case Input::Key::Escape:
		return internal->input.KeyEscape == Input::Status::Release;
	case Input::Key::W:
		return internal->input.KeyW == Input::Status::Release;
	case Input::Key::A:
		return internal->input.KeyA == Input::Status::Release;
	case Input::Key::S:
		return internal->input.KeyS == Input::Status::Release;
	case Input::Key::D:
		return internal->input.KeyD == Input::Status::Release;
	default:
		return false;
	}
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
		internal.input.KeyEscape = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_W:
		internal.input.KeyW = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_A:
		internal.input.KeyA = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_S:
		internal.input.KeyS = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	case GLFW_KEY_D:
		internal.input.KeyD = action == GLFW_PRESS ? Input::Status::Press : Input::Status::Release;
		break;
	}
}
