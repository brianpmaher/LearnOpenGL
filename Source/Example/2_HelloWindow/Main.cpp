#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
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

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
