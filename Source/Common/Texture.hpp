#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture(const char* path, GLenum format = GL_RGB, bool flipY = false);

	~Texture();

	inline void Bind(GLenum slot) const
	{
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	inline GLuint GetID() const
	{
		return id;
	}

private:
	GLuint id;
};
