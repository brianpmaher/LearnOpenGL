#include "Image.hpp"

#include <stb/stb_image.h>

#include <iostream>

Image::Image(const char* path, bool flipY)
{
	std::cout << "Loading image: \"" << path << "\"" << std::endl;

	stbi_set_flip_vertically_on_load(flipY);

	if (!(data = stbi_load(path, &width, &height, &numChannels, 0)))
	{
		std::runtime_error("Failed to load image: " + std::string(path));
	}
}

Image::~Image()
{
	std::cout << "Unloading image" << std::endl;

	stbi_image_free(data);
}
