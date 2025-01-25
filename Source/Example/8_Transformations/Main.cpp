#include "Common/Shader.hpp"
#include "Common/Texture.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define TITLE "Transformations"
#define WORKING_DIR "Source/Example/8_Transformations"

int main()
{
	auto window = Window(800, 600, TITLE);

	auto shader = Shader(WORKING_DIR "/Vertex.glsl", WORKING_DIR "/Fragment.glsl");

	auto containerTexture = Texture(WORKING_DIR "/Container.jpg");

	auto awesomeFaceTexture = Texture(WORKING_DIR "/AwesomeFace.png", GL_RGBA, true);

	GLfloat vertices[] = {
		// positions        // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
		0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
	};
	GLuint indices[] = {
		0, 1, 3, //
		1, 2, 3, //
	};
	constexpr GLint stride = 5 * sizeof(GLfloat);

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
	GLint offset = 0;
	GLint index = 0;
	{
		constexpr GLint size = 3;
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(index);
		index++;
		offset += size;
	}

	// Texture coords
	{
		constexpr GLint size = 2;
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

		auto time = (float)glfwGetTime();
		auto transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(sinf(time) * 0.5, cosf(time) * 0.5, 0.0f));
		transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		containerTexture.Bind(GL_TEXTURE0);
		awesomeFaceTexture.Bind(GL_TEXTURE1);
		shader.SetUniform("uTransform", transform);

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
