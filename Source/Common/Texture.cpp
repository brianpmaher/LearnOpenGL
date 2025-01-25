#include "Texture.hpp"

#include "Image.hpp"

#include <iostream>

Texture::Texture(const char* path, GLenum format, bool flipY)
{
	std::cout << "Creating texture: \"" << path << "\"" << std::endl;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto image = Image(path, flipY);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, format, GL_UNSIGNED_BYTE, image.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	std::cout << "Created texture: " << id << std::endl;
}

Texture::~Texture()
{
	std::cout << "Deleting texture: " << id << std::endl;

	glDeleteTextures(1, &id);
}
