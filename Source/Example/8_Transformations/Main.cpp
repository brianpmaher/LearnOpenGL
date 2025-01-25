#include "Common/Shader.hpp"
#include "Common/Texture.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#define TITLE "Transformations"
#define WORKING_DIR "Source/Example/8_Transformations"

int main()
{
	auto window = Window(800, 600, TITLE);

	auto shader = Shader(WORKING_DIR "/Vertex.glsl", WORKING_DIR "/Fragment.glsl");

	auto containerTexture = Texture(WORKING_DIR "/Container.jpg");

	auto awesomeFaceTexture = Texture(WORKING_DIR "/AwesomeFace.png", GL_RGBA, true);

	float vertices[] = {
		// positions        // colors         // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
	};
	GLuint indices[] = {
		0, 1, 3, //
		1, 2, 3, //
	};
	constexpr int stride = 8 * sizeof(GLfloat);

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
	int offset = 0;
	int index = 0;
	{
		constexpr int size = 3;
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(index);
		index++;
		offset += size;
	}

	// Color attribute
	{
		constexpr int size = 3;
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(index);
		index++;
		offset += size;
	}

	// Texture coords
	{
		constexpr int size = 2;
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(index);
		index++;
		offset += size;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shader.Use();
	shader.SetUniform("texture0", 0);
	shader.SetUniform("texture1", 1);

	while (!window.ShouldClose())
	{
		window.PollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		containerTexture.Bind(GL_TEXTURE0);
		awesomeFaceTexture.Bind(GL_TEXTURE1);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(GL_NONE);

		window.SwapBuffers();
	}

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
