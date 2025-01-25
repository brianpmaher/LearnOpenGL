#pragma once

#include <glad/glad.h>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	~Shader();

	inline void Use()
	{
		glUseProgram(id);
	}

	inline void SetUniform(const char* name, GLboolean value) const
	{
		glUniform1i(glGetUniformLocation(id, name), static_cast<int>(value));
	}

	inline void SetUniform(const char* name, GLint value) const
	{
		glUniform1i(glGetUniformLocation(id, name), value);
	}

	inline void SetUniform(const char* name, GLfloat value) const
	{
		glUniform1f(glGetUniformLocation(id, name), value);
	}

	inline void SetUniform(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
	{
		glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
	}

	inline GLuint GetID() const
	{
		return id;
	}

private:
	GLuint id;
};
