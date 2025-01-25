#include "Common/Shader.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#define TITLE "Shaders"
#define WORKING_DIR "Source/Example/6_Shaders"

int main()
{
	auto window = Window(800, 600, TITLE);

	// Print number of attributes
	{
		int numAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
		std::cout << "GL_MAX_VERTEX_ATTRIBS: " << numAttributes << std::endl;
	}

	auto shader = Shader(WORKING_DIR "/vertex.glsl", WORKING_DIR "/fragment.glsl");

	GLfloat vertices[] = {
		// positions        // colors
		0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // top
	};
	GLuint indices[] = {0, 1, 2};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!window.ShouldClose())
	{
		window.PollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		glBindVertexArray(vao);

		// Update green color with time and sine function
		{
			float time = glfwGetTime();
			float green = (sinf(time) / 2.0f) + 0.5f;
			shader.SetUniform("uColor", 0.0f, green, 0.0f, 0.0f);
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(GL_NONE);

		window.SwapBuffers();
	}

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
