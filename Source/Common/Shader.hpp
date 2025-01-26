#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	inline void SetUniform(const char* name, GLfloat x, GLfloat y, GLfloat z) const
	{
		glUniform3f(glGetUniformLocation(id, name), x, y, z);
	}

	inline void SetUniform(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
	{
		glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
	}

	inline void SetUniform(const char* name, const glm::vec3& vector) const
	{
		glUniform3f(glGetUniformLocation(id, name), vector.x, vector.y, vector.z);
	}

	inline void SetUniform(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	inline GLuint GetID() const
	{
		return id;
	}

private:
	GLuint id;
};
