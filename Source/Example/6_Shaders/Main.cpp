#include "Common/Shader.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

int main()
{
	auto window = Window(800, 600, "Shaders");

	// Print number of attributes
	{
		int numAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
		std::cout << "GL_MAX_VERTEX_ATTRIBS: " << numAttributes << std::endl;
	}

	GLuint shaderProgram;
	{
		shaderProgram = glCreateProgram();

		GLuint vertexShader;
		{
			const char* vertexShaderSource =
				"#version 330 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aColor;\n"
				"out vec3 color;\n"
				"void main()\n"
				"{\n"
				"	gl_Position = vec4(aPos, 1.0);\n"
				"	color = aColor;\n"
				"}\n";
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
			glCompileShader(vertexShader);

			GLint success;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				constexpr int infoLogSize = 512;
				char infoLog[infoLogSize];
				glGetShaderInfoLog(vertexShader, infoLogSize, nullptr, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				return EXIT_FAILURE;
			}
		}

		GLuint fragmentShader;
		{
			const char* fragmentShaderSource =
				"#version 330 core\n"
				"out vec4 FragColor;\n"
				"in vec3 color;\n"
				"void main()\n"
				"{\n"
				"	FragColor = vec4(color, 1.0);\n"
				"}\n";
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
			glCompileShader(fragmentShader);

			GLint success;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				constexpr int infoLogSize = 512;
				char infoLog[infoLogSize];
				glGetShaderInfoLog(fragmentShader, infoLogSize, nullptr, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				return EXIT_FAILURE;
			}
		}

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			constexpr int infoLogSize = 512;
			char infoLog[infoLogSize];
			glGetProgramInfoLog(shaderProgram, infoLogSize, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
			return EXIT_FAILURE;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

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

	glUseProgram(shaderProgram);
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "uColor");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!window.ShouldClose())
	{
		window.PollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		// Update green color with time and sine function
		{
			float time = glfwGetTime();
			float green = (sinf(time) / 2.0f) + 0.5f;
			glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 0.0f);
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(GL_NONE);

		window.SwapBuffers();
	}

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);

	return EXIT_SUCCESS;
}
