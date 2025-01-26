#include "Common/Camera.hpp"
#include "Common/Shader.hpp"
#include "Common/Texture.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define TITLE "Camera"
#define WORKING_DIR "Source/Example/10_Camera"

int main()
{
	auto window = Window(800, 600, TITLE);

	window.LockCursor();

	auto camera = Camera(window, {0.0f, 0.0f, 3.0f});

	auto shader = Shader(WORKING_DIR "/Vertex.glsl", WORKING_DIR "/Fragment.glsl");

	auto containerTexture = Texture(WORKING_DIR "/Container.jpg");

	auto awesomeFaceTexture = Texture(WORKING_DIR "/AwesomeFace.png", GL_RGBA, true);

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
		0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //

		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //

		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //
		-0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, //
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
		0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, //
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
		0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, //
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //

		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
		-0.5f, 0.5f,  0.5f,  0.0f, 0.0f, //
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
	};
	constexpr GLint stride = 5 * sizeof(GLfloat);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

	shader.Use();
	shader.SetUniform("texture0", 0);
	shader.SetUniform("texture1", 1);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
		{ 0.0f,  0.0f,   0.0f}, //
		{ 2.0f,  5.0f, -15.0f}, //
		{-1.5f, -2.2f,  -2.5f}, //
		{-3.8f, -2.0f, -12.3f}, //
		{ 2.4f, -0.4f,  -3.5f}, //
		{-1.7f,  3.0f,  -7.5f}, //
		{ 1.3f, -2.0f,  -2.5f}, //
		{ 1.5f,  2.0f,  -2.5f}, //
		{ 1.5f,  0.2f,  -1.5f}, //
		{-1.3f,  1.0f,  -1.5f}, //
	};

	float prevTime = glfwGetTime();

	while (!window.ShouldClose())
	{
		// Update
		{
			window.PollEvents();

			float deltaTime;
			{
				float time = glfwGetTime();
				deltaTime = time - prevTime;
				prevTime = time;
			}

			if (window.IsKeyPressed(Input::Key::Escape))
			{
				window.Close();
			}

			camera.Update(deltaTime);
		}

		// Render
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.Use();
			containerTexture.Bind(GL_TEXTURE0);
			awesomeFaceTexture.Bind(GL_TEXTURE1);
			shader.SetUniform("view", camera.GetView());
			shader.SetUniform("projection", camera.GetProjection());

			glBindVertexArray(vao);
			for (int i = 0; i < 10; i++)
			{
				auto model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader.SetUniform("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(GL_NONE);

			window.SwapBuffers();
		}
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
