#include "Common/Camera.hpp"
#include "Common/Shader.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define TITLE "Basic Lighting"
#define WORKING_DIR "Source/Example/12_BasicLighting"

int main()
{
	auto window = Window(800, 600, TITLE);

	window.LockCursor();

	auto camera = Camera(window, {0.5f, 1.0f, 5.0f}, -15.0f);

	auto objectShader = Shader(WORKING_DIR "/Object.vert.glsl", WORKING_DIR "/Object.frag.glsl");
	auto lightShader = Shader(WORKING_DIR "/Light.vert.glsl", WORKING_DIR "/Light.frag.glsl");

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, //
		0.5f,  -0.5f, -0.5f, //
		0.5f,  0.5f,  -0.5f, //
		0.5f,  0.5f,  -0.5f, //
		-0.5f, 0.5f,  -0.5f, //
		-0.5f, -0.5f, -0.5f, //

		-0.5f, -0.5f, 0.5f, //
		0.5f,  -0.5f, 0.5f, //
		0.5f,  0.5f,  0.5f, //
		0.5f,  0.5f,  0.5f, //
		-0.5f, 0.5f,  0.5f, //
		-0.5f, -0.5f, 0.5f, //

		-0.5f, 0.5f,  0.5f,  //
		-0.5f, 0.5f,  -0.5f, //
		-0.5f, -0.5f, -0.5f, //
		-0.5f, -0.5f, -0.5f, //
		-0.5f, -0.5f, 0.5f,  //
		-0.5f, 0.5f,  0.5f,  //

		0.5f,  0.5f,  0.5f,  //
		0.5f,  0.5f,  -0.5f, //
		0.5f,  -0.5f, -0.5f, //
		0.5f,  -0.5f, -0.5f, //
		0.5f,  -0.5f, 0.5f,  //
		0.5f,  0.5f,  0.5f,  //

		-0.5f, -0.5f, -0.5f, //
		0.5f,  -0.5f, -0.5f, //
		0.5f,  -0.5f, 0.5f,  //
		0.5f,  -0.5f, 0.5f,  //
		-0.5f, -0.5f, 0.5f,  //
		-0.5f, -0.5f, -0.5f, //

		-0.5f, 0.5f,  -0.5f, //
		0.5f,  0.5f,  -0.5f, //
		0.5f,  0.5f,  0.5f,  //
		0.5f,  0.5f,  0.5f,  //
		-0.5f, 0.5f,  0.5f,  //
		-0.5f, 0.5f,  -0.5f, //
	};

	constexpr GLint stride = 3 * sizeof(GLfloat);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint lightVao;
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);

	// Position attribute
	{
		GLint offset = 0;
		GLint index = 0;
		{
			constexpr GLint size = 3;
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
			glEnableVertexAttribArray(index);
			index++;
			offset += size;
		}
	}

	GLuint objectVao;
	glGenVertexArrays(1, &objectVao);
	glBindVertexArray(objectVao);

	// Position attribute
	{
		GLint offset = 0;
		GLint index = 0;
		{
			constexpr GLint size = 3;
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
			glEnableVertexAttribArray(index);
			index++;
			offset += size;
		}
	}

	auto objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	auto lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
	auto objectColor = glm::vec3(1.0f, 0.5f, 0.32f);
	auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	float prevTime = glfwGetTime();

	while (!window.ShouldClose())
	{
		// Update
		{
			window.PollEvents();

			float deltaTime;
			float time = glfwGetTime();
			deltaTime = time - prevTime;
			prevTime = time;

			if (window.IsKeyPressed(Input::Key::Escape))
			{
				window.Close();
			}

			camera.Update(deltaTime);

			lightColor = glm::vec3((sinf(time) + 1) / 2);
		}

		// Render
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw object
			{
				objectShader.Use();
				auto model = glm::mat4(1.0f);
				model = glm::translate(model, objectPosition);
				objectShader.SetUniform("model", model);
				objectShader.SetUniform("view", camera.GetView());
				objectShader.SetUniform("projection", camera.GetProjection());
				objectShader.SetUniform("objectColor", objectColor);
				objectShader.SetUniform("lightColor", lightColor);
				glBindVertexArray(objectVao);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// Draw light
			{
				lightShader.Use();
				auto model = glm::mat4(1.0f);
				model = glm::translate(model, lightPosition);
				model = glm::scale(model, glm::vec3(0.2f));
				lightShader.SetUniform("model", model);
				lightShader.SetUniform("view", camera.GetView());
				lightShader.SetUniform("projection", camera.GetProjection());
				lightShader.SetUniform("color", lightColor);
				glBindVertexArray(lightVao);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glBindVertexArray(GL_NONE);

			window.SwapBuffers();
		}
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &lightVao);
	glDeleteVertexArrays(1, &objectVao);

	return EXIT_SUCCESS;
}
