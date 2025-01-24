#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <numbers>

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Clear", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// Change color with sine function
		// Source: https://justinparrtech.com/JustinParr-Tech/spectrum-generating-color-function-using-sine-waves/
		{
			constexpr float pi = std::numbers::pi_v<float>;
			float time = (float)glfwGetTime();
			float red = sinf(time) * 2.0f / 2.0f;
			float green = sinf(time - 2.0f * pi / 3.0f) * 2.0f / 2.0f;
			float blue = sinf(time - 4.0f * pi / 3.0f) * 2.0f / 2.0f;
			glClearColor(red, green, blue, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
