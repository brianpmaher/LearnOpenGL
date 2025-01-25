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

	auto cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;
	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto cameraRight = glm::cross(cameraFront, cameraUp);
	auto view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	auto projection = glm::mat4(1.0f);
	constexpr float near = 0.1f;
	constexpr float far = 100.0f;
	projection = glm::perspective(glm::radians(45.0f), window.GetAspect(), near, far);

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
		float deltaTime;
		{
			float time = glfwGetTime();
			deltaTime = time - prevTime;
			prevTime = time;
		}

		window.PollEvents();

		if (window.IsKeyPressed(Input::Key::Escape))
		{
			window.Close();
		}

		// Update camera movement
		{
			// Update keyboard movement
			{
				auto moveDirection = glm::vec3(0.0f);

				// Forward
				if (window.IsKeyDown(Input::Key::W))
				{
					moveDirection = cameraFront;
				}

				// Left
				if (window.IsKeyDown(Input::Key::A))
				{
					moveDirection -= cameraRight;
				}

				// Backward
				if (window.IsKeyDown(Input::Key::S))
				{
					moveDirection -= cameraFront;
				}

				// Right
				if (window.IsKeyDown(Input::Key::D))
				{
					moveDirection += cameraRight;
				}

				if (glm::length(moveDirection) > 0.0f)
				{
					constexpr float moveSpeed = 2.0f;
					cameraPosition += glm::normalize(moveDirection) * moveSpeed * deltaTime;
				}
			}

			// Update mouse rotation
			{
				glm::vec2 cursorMovement = window.GetCursorMovement();
				constexpr float sensitivity = 0.1f;
				glm::vec2 deltaMovement = cursorMovement * sensitivity;

				yaw += deltaMovement.x;
				pitch -= deltaMovement.y;

				pitch = glm::clamp(pitch, -89.0f, 89.0f);

				glm::vec3 direction;
				auto yawRad = glm::radians(yaw);
				auto pitchRad = glm::radians(pitch);
				direction.x = cos(yawRad) * cos(pitchRad);
				direction.y = sin(pitchRad);
				direction.z = sin(yawRad) * cos(pitchRad);
				cameraFront = glm::normalize(direction);
				cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
			}
		}

		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(45.0f), window.GetAspect(), near, far);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		containerTexture.Bind(GL_TEXTURE0);
		awesomeFaceTexture.Bind(GL_TEXTURE1);
		shader.SetUniform("view", view);
		shader.SetUniform("projection", projection);

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

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
