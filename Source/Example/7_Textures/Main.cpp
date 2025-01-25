#include "Common/Image.hpp"
#include "Common/Shader.hpp"
#include "Common/Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#define TITLE "Textures"
#define WORKING_DIR "Source/Example/7_Textures"

int main()
{
	auto window = Window(800, 600, TITLE);

	auto shader = Shader(WORKING_DIR "/Vertex.glsl", WORKING_DIR "/Fragment.glsl");

	GLuint containerTexture;
	{
		glGenTextures(1, &containerTexture);
		glBindTexture(GL_TEXTURE_2D, containerTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		auto image = Image(WORKING_DIR "/Container.jpg");
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			image.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	GLuint awesomeFaceTexture;
	{
		glGenTextures(1, &awesomeFaceTexture);
		glBindTexture(GL_TEXTURE_2D, awesomeFaceTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		constexpr bool flipY = true;
		auto image = Image(WORKING_DIR "/AwesomeFace.png", flipY);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
			image.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, awesomeFaceTexture);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(GL_NONE);

		window.SwapBuffers();
	}

	glDeleteTextures(1, &containerTexture);
	glDeleteTextures(1, &awesomeFaceTexture);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
